#include <Arduino.h>
#include <t3spi.h>
#include <Config.h>
#include <Pins.h>
#include <Slave.h>
#include <Common.h>
#include <IMU.h>
#include <SPI.h>
#include <Timer.h>
#include <PID.h>
#include <BallData.h>
#include <LineData.h>
#include <MoveData.h>
#include <Camera.h>
#include <Bluetooth.h>
#include <BluetoothData.h>
#include <PlayMode.h>
#include <Point.h>
#include <EEPROM.h>

T3SPI spi;

SlaveSensor slaveSensor;
SlaveMotor slaveMotor;
SlaveDebug slaveDebug;

Bluetooth bluetooth;
BluetoothData bluetoothData;
Timer bluetoothTimer(BLUETOOTH_UPDATE_TIME);

IMUFusion imu;
Camera camera;

DebugSettings settings;

LineData lineData(0, 0, true);
BallData ballData;
MoveData moveData;

Point robotPosition;

int facingDirection = 0;
int attackingGoalAngle = 0;
int defendingGoalAngle = 0;

bool facingGoal = false;

Timer ledTimer(LED_BLINK_TIME_MASTER);
Timer slaveDebugUpdateTimer(SLAVE_DEBUG_UPDATE_TIME);

PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
PID goalHeadingAttackPID(GOAL_HEADING_ATTACK_KP, GOAL_HEADING_ATTACK_KI, GOAL_HEADING_ATTACK_KD, GOAL_HEADING_ATTACK_MAX_CORRECTION);
PID coordinatePID(MOVE_TO_COORDINATE_KP, MOVE_TO_COORDINATE_KI, MOVE_TO_COORDINATE_KD, MOVE_TO_COORDINATE_MAX_SPEED);
PID defendPID(DEFEND_BALL_ANGLE_KP, DEFEND_BALL_ANGLE_KI, DEFEND_BALL_ANGLE_KD, MAX_DEFEND_MOVEMENT_X);

PlayMode playMode = PlayMode::undecidedMode;
PlayMode defaultPlayMode;

bool ledOn;

bool movingSideways = false;
Point sidewaysCoordinate;

// Robot ID:
// 0: Default attacker
// 1: Default defender - switching master
uint8_t robotID;

PlayMode currentPlayMode() {
    if (bluetooth.isConnected) {
        return playMode == PlayMode::undecidedMode ? defaultPlayMode : playMode;
    } else if (bluetooth.previouslyConnected) {
        return PlayMode::defendMode;
    } else {
        return settings.defaultPlayModeIsAttack ? PlayMode::attackMode : PlayMode::defendMode;
    }
}

bool attackingGoalVisible() {
    return settings.goalIsYellow ? camera.yellowGoalVisible() : camera.blueGoalVisible();
}

bool defendingGoalVisible() {
    return !settings.goalIsYellow ? camera.yellowGoalVisible() : camera.blueGoalVisible();
}

void updateLine(double angle, double size) {
    bool noLine = angle == NO_LINE_ANGLE || size == 3;

    angle = noLine ? 0 : doubleMod(angle + imu.getHeading(), 360);

    if (lineData.onField) {
        if (!noLine) {
            lineData.angle = angle;
            lineData.size = size;

            lineData.onField = false;
        }
    } else {
        if (lineData.size == 3) {
            if (!noLine) {
                lineData.angle = doubleMod(angle + 180, 360);
                lineData.size = 2 - size;
            }
        } else {
            if (noLine) {
                if (lineData.size <= 1) {
                    lineData.onField = true;
                    lineData.size = 0;
                    lineData.angle = 0;
                } else {
                    lineData.size = 3;
                }
            } else {
                if (smallestAngleBetween(lineData.angle, angle) <= 90) {
                    lineData.angle = angle;
                    lineData.size = size;
                } else {
                    lineData.angle = doubleMod(angle + 180, 360);
                    lineData.size = 2 - size;
                }
            }
        }
    }
}

bool isOutsideLine(double angle) {
    if (lineData.onField) {
        return false;
    }

    if (mod(lineData.angle, 90) > LINE_CORNER_ANGLE_THRESHOLD && mod(lineData.angle, 90) < 90 - LINE_CORNER_ANGLE_THRESHOLD) {
        return (angleIsInside(doubleMod(lineData.angle - 135 - LINE_ANGLE_BUFFER_CORNER, 360), doubleMod(lineData.angle + 135 + LINE_ANGLE_BUFFER_CORNER, 360), mod(angle + imu.getHeading(), 360)));
    } else {
        return (angleIsInside(doubleMod(lineData.angle - 90 - LINE_ANGLE_BUFFER, 360), doubleMod(lineData.angle + 90 + LINE_ANGLE_BUFFER, 360), mod(angle + imu.getHeading(), 360)));
    }
}

void calculateLineAvoid() {
    if (!lineData.onField) {
        if (lineData.size > LINE_BIG_SIZE) {
            moveData.angle = mod(lineData.angle + 180 - imu.getHeading(), 360);
            moveData.speed = lineData.size == 3 ? OVER_LINE_SPEED : min(lineData.size / 2.0 * LINE_SPEED * 5, LINE_SPEED);
        } else if ((lineData.size > LINE_SMALL_SIZE) && isOutsideLine(moveData.angle)) {
            moveData.angle = 0;
            moveData.speed = 0;
        }
    }
}

void calculateOrbit() {
    double attackingGoalAngleAddition = FORWARD_GOAL_TRACKING && ballData.strength > GOAL_TRACK_SHORT_STRENGTH ? attackingGoalAngle : 0;
    double ballAngle = mod(ballData.angle - attackingGoalAngleAddition, 360);
    double ballAngleDifference = -sign(ballData.angle - 180) * fmin(90, 0.4 * pow(MATH_E, 0.15 * (double)smallestAngleBetween(ballData.angle, 0)));
    double distanceMultiplier = constrain(0.02 * ballData.strengthFactor() * pow(MATH_E, 4.5 * ballData.strengthFactor()), 0, 1);
    double angleAddition = ballAngleDifference * distanceMultiplier;
    moveData.angle = mod(ballData.angle + angleAddition + attackingGoalAngleAddition, 360);
    moveData.speed = ORBIT_SPEED_SLOW + (double)(ORBIT_SPEED_FAST - ORBIT_SPEED_SLOW) * (1.0 - (double)abs(angleAddition) / 90.0);
}

bool moveByDifference(Point difference) {
    if (difference.getMagnitude() < AT_COORDINATE_THRESHOLD_DISTANCE) {
        moveData.angle = 0;
        moveData.speed = 0;

        return true;
    } else {
        moveData.angle = mod(difference.getAngle() - imu.getHeading(), 360);
        moveData.speed = abs(coordinatePID.update(difference.getMagnitude(), 0));
    }

    return false;
}

bool moveToCoordinate(Point point) {
    if (camera.goalsVisible()) {
        return moveByDifference(point - robotPosition);
    } else {
        moveData.angle = 0;
        moveData.speed = 0;
    }

    return false;
}

void goalTracking() {
    if (attackingGoalVisible() && GOAL_TRACKING) {
        facingDirection = ALWAYS_FACE_GOAL ? attackingGoalAngle : mod((double)(mod(attackingGoalAngle + 180, 360) - 180) * constrain(0.00000005 * ballData.strengthFactor() * pow(MATH_E, 20 * ballData.strengthFactor()), 0, 1), 360);
        facingGoal = true;
    } else {
        facingGoal = false;
    }
}

void updateCamera() {
    camera.update();

    if (camera.goalsVisible()) {
        if (camera.newData()) {
            int angle = camera.yellowClosest() ? camera.yellowAngle : camera.blueAngle;
            double distance = camera.shortestDistance();

            angle = mod(angle + imu.getHeading(), 360);

            robotPosition.x = constrain(distance * -sin(degreesToRadians(angle)), -(FIELD_WIDTH_CENTIMETERS / 2), FIELD_WIDTH_CENTIMETERS / 2);
            robotPosition.y = (FIELD_LENGTH_CENTIMETERS / 2 - GOAL_EDGE_OFFSET_CENTIMETERS) * (settings.goalIsYellow && camera.yellowClosest() || !settings.goalIsYellow && !camera.yellowClosest() ? 1 : -1) + distance * -cos(degreesToRadians(angle));

            attackingGoalAngle = settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
            defendingGoalAngle = !settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
        }
    }
}

void avoidDoubleDefence() {
    if (bluetooth.isConnected) {
        int moveAngle = moveData.angle;
        int moveSpeed = moveData.speed;

        if (angleIsInside(90, 270, moveAngle) && bluetooth.otherData.robotPosition.y < DOUBLE_DEFENCE_MIN_Y) {
            if (robotPosition.y < DOUBLE_DEFENCE_MIN_Y && moveToCoordinate(Point(robotPosition.x, DOUBLE_DEFENCE_MIN_Y))) {
                moveData.speed = moveSpeed;
                moveData.angle = smallestAngleBetween(180, moveAngle) * -sign(moveAngle - 180);
            }
        }  
    }
}

void attack() {
    if (movingSideways) {
        if ((ballData.visible() || bluetooth.otherData.ballData.visible()) && !bluetooth.otherData.ballIsOut) {
            movingSideways = !moveToCoordinate(sidewaysCoordinate);
        } else {
            movingSideways = false;
        }

        facingGoal = false;
    } else if (bluetooth.otherData.ballIsOut) {
        moveToCoordinate(Point(BALL_OUT_CENTRE_X, BALL_OUT_CENTRE_Y));
        facingGoal = false;
    } else if (ballData.visible()) {
        goalTracking();

        calculateOrbit();
    } else {
        moveToCoordinate(Point(NO_BALL_CENTRE_X, NO_BALL_CENTRE_Y));
        facingGoal = false;
    }
}

void defend() {
    if (camera.goalsVisible()) {
        Point goalPosition = Point(0, -(FIELD_LENGTH_CENTIMETERS / 2) + GOAL_EDGE_OFFSET_CENTIMETERS);

        if (ballData.visible()) {
            Point relativeBallPosition = ballData.position(imu.getHeading());

            if (ballData.strength > DEFEND_CHARGE_STRENGTH && angleIsInside(360 - DEFEND_CHARGE_ANGLE, DEFEND_CHARGE_ANGLE, ballData.angle) && robotPosition.y < DEFEND_CHARGE_MAX_Y) {
                moveData.speed = DEFEND_CHARGE_SPEED;
                moveData.angle = 0;
            } else if (relativeBallPosition.y < 0) {
                if (robotPosition.y < goalPosition.y + DEFEND_GOAL_DISTANCE_ORBIT) {
                    moveToCoordinate(Point(MAX_DEFEND_X * sign(relativeBallPosition.x), goalPosition.y + DEFEND_GOAL_DISTANCE_CLOSE));
                    facingDirection = mod(90 * sign(relativeBallPosition.x), 360);
                } else {
                    calculateOrbit();
                }

                facingGoal = false;
            } else {
                double ballAngle = mod(ballData.angle - (180 + defendingGoalAngle), 360);
                double xDifference = defendPID.update(smallestAngleBetween(ballAngle, 0) * -sign(ballAngle - 180), 0);

                moveByDifference(Point(abs(robotPosition.x) > MAX_DEFEND_X && sign(xDifference) == sign(robotPosition.x) ? MAX_DEFEND_X * sign(robotPosition.x) - robotPosition.x : xDifference, goalPosition.y + DEFEND_GOAL_DISTANCE - robotPosition.y));
            }
        } else {
            moveToCoordinate(Point(0, goalPosition.y + DEFEND_GOAL_DISTANCE));
            facingGoal = false;
        }
    } else {
        if (ballData.visible()) {
            calculateOrbit();
            facingGoal = false;
        } else {
            moveData.speed = 0;
            moveData.angle = 0;
        }
    }

    facingGoal = false;
}

void calculateMovement() {
    if (currentPlayMode() == PlayMode::attackMode) {
        attack();
    } else {
        defend();
    }

    calculateLineAvoid();

    if (facingGoal) {
        moveData.rotation = (int8_t)round(goalHeadingAttackPID.update(doubleMod(doubleMod(imu.getHeading() - facingDirection, 360) + 180, 360) - 180, 0));
    } else {
        moveData.rotation = (int8_t)round(headingPID.update(doubleMod(imu.getHeading() + 180, 360) - 180, 0));
    }
}

void updateDebug() {
    slaveDebug.updateDebugSettings();
    settings = slaveDebug.debugSettings;

    slaveDebug.sendPlayMode(currentPlayMode() == PlayMode::attackMode);
    slaveDebug.sendBluetoothConnected(bluetooth.isConnected);

    if (!settings.gameMode) {
        slaveDebug.sendBallData(ballData);
        slaveDebug.sendHeading(imu.getHeading());

        slaveMotor.updateLeftRPM();
        slaveMotor.updateRightRPM();
        slaveMotor.updateBackLeftRPM();
        slaveMotor.updateBackRightRPM();

        slaveDebug.sendLeftRPM(slaveMotor.leftRPM);
        slaveDebug.sendRightRPM(slaveMotor.rightRPM);
        slaveDebug.sendBackLeftRPM(slaveMotor.backLeftRPM);
        slaveDebug.sendBackRightRPM(slaveMotor.backRightRPM);

        slaveSensor.updateLightSensorData();

        slaveDebug.sendLineData(lineData);
        slaveDebug.sendLightSensorData(slaveSensor.lsFirst, slaveSensor.lsSecond, slaveSensor.lsThird, slaveSensor.lsFourth);

        slaveDebug.sendGoals(camera.yellowAngle, camera.yellowPixelDistance, camera.blueAngle, camera.bluePixelDistance);

        slaveDebug.sendRobotPosition(robotPosition);

        if (bluetooth.isConnected) {
            slaveDebug.sendBluetoothData(bluetooth.otherData);
        }
    }

    if (settings.IMUNeedsResetting && settings.lightSensorsNeedResetting) {
        slaveMotor.brake();

        imu.calibrate();
        imu.resetHeading();
        slaveSensor.sendCalibrateLightSensors();
        lineData = LineData();

        delay(500);

        slaveDebug.sendLightSensorsAreReset();
        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();

        delay(500);
    } else if (settings.IMUNeedsResetting) {
        slaveMotor.brake();

        imu.calibrate();
        imu.resetHeading();

        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();

        delay(500);
    } else if (settings.lightSensorsNeedResetting) {
        slaveMotor.brake();

        slaveSensor.sendCalibrateLightSensors();
        lineData = LineData();

        delay(500);

        slaveDebug.sendLightSensorsAreReset();
        slaveDebug.updateDebugSettings();

        delay(500);
    }

    slaveDebug.updateDebugSettings();
}

bool shouldSwitchPlayMode(BluetoothData attackerData, BluetoothData defenderData) {
    return angleIsInside(360 - PLAYMODE_SWITCH_DEFENDER_ANGLE, PLAYMODE_SWITCH_DEFENDER_ANGLE, defenderData.ballData.angle) 
    && defenderData.ballData.strength > PLAYMODE_SWITCH_DEFENDER_STRENGTH 
    && attackerData.ballData.strength < PLAYMODE_SWITCH_ATTACKER_STRENGTH 
    && (angleIsInside(360 - PLAYMODE_SWITCH_ATTACKER_ANGLE, PLAYMODE_SWITCH_ATTACKER_ANGLE, attackerData.ballData.angle) || attackerData.ballData.strength < PLAYMODE_SWITCH_ATTACKER_STRENGTH_FAR) 
    && attackerData.isOnField 
    && defenderData.isOnField;
}

void updatePlayMode() {
    PlayMode previousPlayMode = playMode;

    if (playMode == PlayMode::undecidedMode) {
        if (bluetooth.otherData.playMode == PlayMode::undecidedMode) {
            playMode = defaultPlayMode;
        } else {
            playMode = bluetooth.otherData.playMode == PlayMode::attackMode ? PlayMode::defendMode : PlayMode::attackMode;
        }
    } else if (robotID == 1) {
        // Robot ID 1 (default defender) decides on play mode

        BluetoothData attackerData = playMode == PlayMode::attackMode ? bluetoothData : bluetooth.otherData;
        BluetoothData defenderData = playMode == PlayMode::defendMode ? bluetoothData : bluetooth.otherData;

        if (shouldSwitchPlayMode(attackerData, defenderData)) {
            playMode = playMode == PlayMode::attackMode ? PlayMode::defendMode : PlayMode::attackMode;
        }
    } else {
        playMode = bluetooth.otherData.playMode == PlayMode::attackMode ? PlayMode::defendMode : PlayMode::attackMode;
    }

    if (playMode != previousPlayMode && previousPlayMode == PlayMode::attackMode) {
        movingSideways = true;
        sidewaysCoordinate = Point(40 * sign(robotPosition.x), robotPosition.y);
    } else {
        movingSideways = false;
    }
}

void updateBluetooth() {
    if (settings.playModeSwitching) {
        bluetoothData = BluetoothData(ballData, imu.getHeading(), isOutsideLine(ballData.angle), playMode, lineData.onField, robotPosition);
        bluetooth.update(bluetoothData);

        if (bluetooth.isConnected) {
            updatePlayMode();
        } else if (bluetooth.previouslyConnected) {
            playMode = PlayMode::defendMode;
        }
    } else {
        bluetooth.disconnect();
    }
}

void setup() {
    #if WRITE_ROBOT_ID_EEPROM
        EEPROM.write(ROBOT_ID_EEPROM_ADDRESS, ROBOT_ID_WRITE);
    #endif

    robotID = EEPROM.read(ROBOT_ID_EEPROM_ADDRESS);

    defaultPlayMode = robotID == 0 ? PlayMode::attackMode : PlayMode::defendMode;

    Serial.begin(9600);

    bluetooth.init();

    spi.begin_MASTER(MASTER_SCK, MASTER_MOSI, MASTER_MISO, MASTER_CS_MOTOR, CS_ActiveLOW);
    spi.setCTAR(CTAR_0, 16, SPI_MODE0, LSB_FIRST, SPI_CLOCK_DIV16);

    slaveSensor.init();
    slaveMotor.init();
    slaveDebug.init();

    imu.init();
    camera.init();

    updateDebug();

	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    slaveSensor.updateBallData();
    ballData = slaveSensor.ballData();

    slaveSensor.updateLineAngle();
    slaveSensor.updateLineSize();

    updateLine(slaveSensor.lineAngle, slaveSensor.lineSize);

    imu.update();

    #if CAMERA_ENABLED
        updateCamera();
    #endif

    if (bluetoothTimer.timeHasPassed()) {
        updateBluetooth();
    }

    calculateMovement();

    if (settings.engineStarted) {
        slaveMotor.setMotor(moveData);
    } else {
        slaveMotor.brake();
    }

    if (slaveDebugUpdateTimer.timeHasPassed()) {
        updateDebug();
    }

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}
