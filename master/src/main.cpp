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

Timer ledTimer(LED_BLINK_TIME_MASTER);
Timer slaveDebugUpdateTimer(SLAVE_DEBUG_UPDATE_TIME);

PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);

PlayMode playMode;

bool ledOn;

bool movingSideways = false;
Point sidewaysCoordinate;

// Robot ID:
// 0: Default attacker 
// 1: Default defender - switching master
uint8_t robotID;

PlayMode currentPlayMode() {
    if (settings.gameMode || settings.playModeSwitching) {
        return playMode;
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
        } else if (lineData.size > LINE_SMALL_SIZE) {
            if (isOutsideLine(moveData.angle)) {
                moveData.angle = 0;
                moveData.speed = 0;
            }
        }
    }
}

void calculateOrbit() {
    if (angleIsInside(360 - ORBIT_SMALL_ANGLE, ORBIT_SMALL_ANGLE, ballData.angle)) {
        moveData.angle = (int)round(ballData.angle < 180 ? (ballData.angle * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) : (360 - (360 - ballData.angle) * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER));
    } else if (angleIsInside(360 - ORBIT_BIG_ANGLE, ORBIT_BIG_ANGLE, ballData.angle)) {
        if (ballData.angle < 180) {
            double nearFactor = (double)(ballData.angle - ORBIT_SMALL_ANGLE) / (double)(ORBIT_BIG_ANGLE - ORBIT_SMALL_ANGLE);
            moveData.angle = (int)round(90 * nearFactor + ballData.angle * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER + ballData.angle * (1 - ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) * nearFactor);
        } else {
            double nearFactor = (double)(360 - ballData.angle - ORBIT_SMALL_ANGLE) / (double)(ORBIT_BIG_ANGLE - ORBIT_SMALL_ANGLE);
            moveData.angle = (int)round(360 - (90 * nearFactor + (360 - ballData.angle) * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER + (360 - ballData.angle) * (1 - ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) * nearFactor));
        }
    } else {
        if (ballData.strength > ORBIT_SHORT_STRENGTH) {
            moveData.angle = ballData.angle + (ballData.angle < 180 ? 90 : -90);
        } else if (ballData.strength > ORBIT_BIG_STRENGTH) {
            double strengthFactor = (double)(ballData.strength - ORBIT_BIG_STRENGTH) / (double)(ORBIT_SHORT_STRENGTH - ORBIT_BIG_STRENGTH);
            double angleFactor = strengthFactor * 90;
            moveData.angle = ballData.angle + (ballData.angle < 180 ? angleFactor : -angleFactor);
        } else {
            moveData.angle = ballData.angle;
        }
    }

    if (angleIsInside(360 - ORBIT_SMALL_ANGLE, ORBIT_SMALL_ANGLE, ballData.angle)) {
        moveData.speed = ATTACK_ORBIT_SPEED;
    } else if (ballData.strength > ORBIT_SHORT_STRENGTH) {
        moveData.speed = FRONT_ORBIT_SPEED + (double)(CLOSE_ORBIT_SPEED - FRONT_ORBIT_SPEED) * (double)abs(mod(attackingGoalAngle + 180, 360) - 180) / 180.0;
    } else if (ballData.strength > ORBIT_BIG_STRENGTH) {
        moveData.speed = CLOSE_ORBIT_SPEED + (double)(FAR_ORBIT_SPEED - CLOSE_ORBIT_SPEED) * (1.0 - (double)(ballData.strength - ORBIT_BIG_STRENGTH) / (double)(ORBIT_SHORT_STRENGTH - ORBIT_BIG_STRENGTH));
    } else {
        moveData.speed = FAR_ORBIT_SPEED;
    }
}

bool moveToCoordinate(Point point) {
    if (camera.goalsVisible()) {
        Point difference = point - robotPosition;

        moveData.angle = mod(difference.getAngle() - imu.getHeading(), 360);
        moveData.speed = constrain(difference.getMagnitude() * MOVE_TO_COORDINATE_SPEED_MULTIPLIER, 0, MOVE_TO_COORDINATE_MAX_SPEED);

        return difference.getMagnitude() < AT_COORDINATE_THRESHOLD_DISTANCE;
    } else {
        moveData.angle = 0;
        moveData.speed = 0;
    }

    return false;
}

void goalTracking() {
    if (attackingGoalVisible()) {
        int goalAngle = mod(attackingGoalAngle + 180, 360) - 180;

        if (!ballData.visible()) {
            facingDirection = 0;
        } else if (ballData.strength > GOAL_TRACK_SHORT_STRENGTH) {
            facingDirection = goalAngle;
        } else if (ballData.strength > GOAL_TRACK_FAR_STRENGTH) {
            facingDirection = ((double)(ballData.strength - GOAL_TRACK_FAR_STRENGTH) / (double)(GOAL_TRACK_SHORT_STRENGTH - GOAL_TRACK_FAR_STRENGTH)) * (double)goalAngle;
        } else {
            facingDirection = 0;
        }

        facingDirection = mod(facingDirection, 360);
    } else {
        facingDirection = 0;
    }
}

void updateCamera() {
    camera.update();

    if (camera.goalsVisible()) {
        if (camera.newData()) {
            int angle = camera.shortestDistance() == camera.yellowCentimeterDistance() ? camera.yellowAngle : camera.blueAngle;
            double distance = camera.shortestDistance();

            angle = mod(angle + imu.getHeading(), 360);

            robotPosition.x = constrain(distance * -sin(degreesToRadians(angle)), -(FIELD_WIDTH_CENTIMETERS / 2), FIELD_WIDTH_CENTIMETERS / 2);
            robotPosition.y = FIELD_LENGTH_CENTIMETERS / 2 - doubleMod(distance * cos(degreesToRadians(angle)) + GOAL_EDGE_OFFSET_CENTIMETERS * sign(cos(degreesToRadians(angle))), FIELD_LENGTH_CENTIMETERS);

            attackingGoalAngle = settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
            defendingGoalAngle = !settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
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
    } else if (bluetooth.otherData.ballIsOut) {
        moveToCoordinate(Point(BALL_OUT_CENTRE_X, BALL_OUT_CENTRE_Y));
    } else if (ballData.visible()) {
        #if CAMERA_ENABLED
            goalTracking();
        #endif
            
        calculateOrbit();
    } else {
        moveToCoordinate(Point(NO_BALL_CENTRE_X, NO_BALL_CENTRE_Y));
    }

    if (camera.goalsVisible() && bluetooth.isConnected && bluetooth.otherData.robotPosition.y < -(FIELD_LENGTH_CENTIMETERS / 2 - GOAL_EDGE_OFFSET_CENTIMETERS - DEFEND_BOX_WIDTH_CENTIMETERS) && robotPosition.y < -(FIELD_LENGTH_CENTIMETERS / 2 - GOAL_EDGE_OFFSET_CENTIMETERS - DEFEND_BOX_WIDTH_CENTIMETERS)) {
        moveToCoordinate(Point(robotPosition.x, -(FIELD_LENGTH_CENTIMETERS / 2 - GOAL_EDGE_OFFSET_CENTIMETERS - DEFEND_BOX_WIDTH_CENTIMETERS) + 10));
    }
}

void defend() {
    if (camera.goalsVisible()) {
        Point goalPosition = Point(0, -(FIELD_LENGTH_CENTIMETERS / 2) + GOAL_EDGE_OFFSET_CENTIMETERS);

        if (ballData.visible()) {
            Point relativeBallPosition = ballData.position(imu.getHeading());

            if (relativeBallPosition.y < 0) {
                moveToCoordinate(Point(MAX_DEFEND_X * sign(relativeBallPosition.x), goalPosition.y + DEFEND_GOAL_DISTANCE));
                facingDirection = mod(90 * sign(relativeBallPosition.x), 360);
            } else {
                Point ballPosition = ballData.position(imu.getHeading()) + robotPosition;
                Point difference = ballPosition - goalPosition;

                Point defendCoordinate = Point(constrain(DEFEND_GOAL_DISTANCE * difference.x / difference.y, -MAX_DEFEND_X, MAX_DEFEND_X), goalPosition.y + DEFEND_GOAL_DISTANCE);

                moveToCoordinate(defendCoordinate);
                facingDirection = ballData.angle + imu.getHeading();
            }
        } else {
            moveToCoordinate(Point(0, goalPosition.y + DEFEND_GOAL_DISTANCE));
            facingDirection = 0;
        }
    } else {
        if (ballData.visible()) {
            calculateOrbit();
            facingDirection = 0;
        } else {
            moveData.speed = 0;
            moveData.angle = 0;
        }
    }
}

void calculateMovement() {
    if (currentPlayMode() == PlayMode::attackMode) {
        attack();
    } else {
        defend();
    }

    calculateLineAvoid();

    moveData.rotation = (int8_t)round(headingPID.update(doubleMod(doubleMod(imu.getHeading() - facingDirection, 360) + 180, 360) - 180, 0));
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

    if (settings.IMUNeedsResetting) {
        slaveMotor.brake();
        imu.calibrate();
        imu.resetHeading();
        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();
        delay(500);
    }

    if (settings.lightSensorsNeedResetting) {
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
    return angleIsInside(360 - PLAYMODE_SWITCH_DEFENDER_ANGLE, PLAYMODE_SWITCH_DEFENDER_ANGLE, defenderData.ballData.angle) && defenderData.ballData.strength > PLAYMODE_SWITCH_DEFENDER_STRENGTH && attackerData.ballData.strength < PLAYMODE_SWITCH_ATTACKER_STRENGTH && (angleIsInside(360 - PLAYMODE_SWITCH_ATTACKER_ANGLE, PLAYMODE_SWITCH_ATTACKER_ANGLE, attackerData.ballData.angle) || attackerData.ballData.strength < PLAYMODE_SWITCH_ATTACKER_STRENGTH_FAR) && attackerData.isOnField && defenderData.isOnField;
}

void updatePlayMode() {
    PlayMode previousPlayMode = playMode;

    if (robotID == 1) {
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
    if (settings.gameMode || settings.playModeSwitching) {
        bluetoothData = BluetoothData(ballData, imu.getHeading(), isOutsideLine(ballData.angle), playMode, lineData.onField, robotPosition);
        bluetooth.update(bluetoothData);

        if (bluetooth.isConnected) {
            updatePlayMode();
        } else {
            if (bluetooth.previouslyConnected) {
                playMode = PlayMode::defendMode;
            }
        }
    } else {
        bluetooth.disconnect();
    }
}

void setup() {
    #if WRITE_ROBOT_ID_EEPROM
        EEPROM.write(ROBOT_ID_EEPROM_ADDRESS, 1);
    #endif

    robotID = EEPROM.read(ROBOT_ID_EEPROM_ADDRESS);
    
    playMode = robotID == 0 ? PlayMode::attackMode : PlayMode::defendMode;

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
    slaveSensor.updateLineAngle();
    slaveSensor.updateLineSize();

    ballData = slaveSensor.ballData();

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
