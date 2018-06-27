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

// SPI for slave communication
T3SPI spi;

// Slave objects
SlaveSensor slaveSensor;
SlaveMotor slaveMotor;
SlaveDebug slaveDebug;

// Bluetooth for inter robot communication
Bluetooth bluetooth;
BluetoothData bluetoothData;
Timer bluetoothTimer(BLUETOOTH_UPDATE_TIME);

// IMU and camera
IMUFusion imu;
Camera camera;

// Touch screen settings
DebugSettings settings;

// Sensor data
LineData lineData(0, 0, true);
BallData ballData;

// Movement
MoveData moveData;

// Robot position
Point robotPosition;

// Direction the robot should face (as a bearing)
int facingDirection = 0;

// Attacking and defending goal angles (absolute)
int attackingGoalAngle = 0;
int defendingGoalAngle = 0;

// Facing the goal
bool facingGoal = false;

// Timers
Timer ledTimer(LED_BLINK_TIME_MASTER);
Timer slaveDebugUpdateTimer(SLAVE_DEBUG_UPDATE_TIME);

// PIDs
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
PID goalHeadingAttackPID(GOAL_HEADING_ATTACK_KP, GOAL_HEADING_ATTACK_KI, GOAL_HEADING_ATTACK_KD, GOAL_HEADING_ATTACK_MAX_CORRECTION);
PID coordinatePID(MOVE_TO_COORDINATE_KP, MOVE_TO_COORDINATE_KI, MOVE_TO_COORDINATE_KD, MOVE_TO_COORDINATE_MAX_SPEED);
PID defendPID(DEFEND_BALL_ANGLE_KP, DEFEND_BALL_ANGLE_KI, DEFEND_BALL_ANGLE_KD, MAX_DEFEND_MOVEMENT_X);

// Play mode
PlayMode playMode = PlayMode::undecidedMode;
PlayMode defaultPlayMode;

// LED
bool ledOn;

// Moving sideways when switching to attack
bool movingSideways = false;
Point sidewaysCoordinate;

// Robot ID:
// 0: Default attacker
// 1: Default defender - switching master
uint8_t robotID;

PlayMode currentPlayMode() {
    // Current play mode, will only return attack or defend

    if (bluetooth.isConnected) {
        // If the playmode hasn't been decided, pick the default otherwise return the playmode
        return playMode == PlayMode::undecidedMode ? defaultPlayMode : playMode;
    } else if (bluetooth.previouslyConnected) {
        // Go to defence if the other has disconnected
        return PlayMode::defendMode;
    } else {
        // Use the touchscreen setting
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
    // Compute the robot's position on the line using the current and previous line data
    bool noLine = angle == NO_LINE_ANGLE || size == 3;

    angle = noLine ? 0 : doubleMod(angle + imu.getHeading(), 360);

    if (lineData.onField) {
        if (!noLine) {
            // On the field and touching the line
            lineData.angle = angle;
            lineData.size = size;

            lineData.onField = false;
        }
    } else {
        // Currently on the line

        if (lineData.size == 3) {
            if (!noLine) {
                // Over the line and touching the line
                lineData.angle = doubleMod(angle + 180, 360);
                lineData.size = 2 - size;
            }
        } else {
            if (noLine) {
                // No line, but was on the line

                if (lineData.size <= 1) {
                    // Was on the inside of the line, is now in the field
                    lineData.onField = true;
                    lineData.size = 0;
                    lineData.angle = 0;
                } else {
                    // Was on the outside of the line, is now outside the field
                    lineData.size = 3;
                }
            } else {
                // Was on the line, and still is on the line

                if (smallestAngleBetween(lineData.angle, angle) <= 90) {
                    // Robot on the inside of the line since the position angle and the line angle are within 90 degrees of one another
                    lineData.angle = angle;
                    lineData.size = size;
                } else {
                    // Angle changed by more than 90, robot on the outside of the line
                    lineData.angle = doubleMod(angle + 180, 360);
                    lineData.size = 2 - size;
                }
            }
        }
    }
}

bool isOutsideLine(double angle) {
    // Compute if an angle is outside the line

    if (lineData.onField) {
        // Not seeing the line, can't tell
        return false;
    }

    if (mod(lineData.angle, 90) > LINE_CORNER_ANGLE_THRESHOLD && mod(lineData.angle, 90) < 90 - LINE_CORNER_ANGLE_THRESHOLD) {
        // On a corner
        return (angleIsInside(doubleMod(lineData.angle - 135 - LINE_ANGLE_BUFFER_CORNER, 360), doubleMod(lineData.angle + 135 + LINE_ANGLE_BUFFER_CORNER, 360), mod(angle + imu.getHeading(), 360)));
    } else {
        // On a side
        return (angleIsInside(doubleMod(lineData.angle - 90 - LINE_ANGLE_BUFFER, 360), doubleMod(lineData.angle + 90 + LINE_ANGLE_BUFFER, 360), mod(angle + imu.getHeading(), 360)));
    }
}

void calculateLineAvoid() {
    if (!lineData.onField) {
        if (lineData.size > LINE_BIG_SIZE) {
            // Most of the way off the line, go back in fast
            moveData.angle = mod(lineData.angle + 180 - imu.getHeading(), 360);
            moveData.speed = lineData.size == 3 ? OVER_LINE_SPEED : min(lineData.size / 2.0 * LINE_SPEED * 5, LINE_SPEED);
        } else if ((lineData.size > LINE_SMALL_SIZE) && isOutsideLine(moveData.angle)) {
            // Sit still on the line if the movement wants to go outside the line
            moveData.angle = 0;
            moveData.speed = 0;
        }
    }
}

void calculateOrbit() {
    // Orbit based on ball angle and strength
    
    // Add on an angle to the ball angle depending on the ball's angle. Exponential function
    double ballAngleDifference = -sign(ballData.angle - 180) * fmin(90, 0.4 * pow(MATH_E, 0.15 * (double)smallestAngleBetween(ballData.angle, 0)));

    // Multiply the addition by distance. The further the ball, the more the robot moves towards the ball. Also an exponential function
    double distanceMultiplier = constrain(0.02 * ballData.strengthFactor() * pow(MATH_E, 4.5 * ballData.strengthFactor()), 0, 1);
    double angleAddition = ballAngleDifference * distanceMultiplier;

    moveData.angle = mod(ballData.angle + angleAddition, 360);
    moveData.speed = ORBIT_SPEED_SLOW + (double)(ORBIT_SPEED_FAST - ORBIT_SPEED_SLOW) * (1.0 - (double)abs(angleAddition) / 90.0);
}

bool moveByDifference(Point difference) {
    if (difference.getMagnitude() < AT_COORDINATE_THRESHOLD_DISTANCE) {
        // If at the coordinate, stop
        moveData.angle = 0;
        moveData.speed = 0;

        return true;
    } else {
        // Move using PID with difference
        moveData.angle = mod(difference.getAngle() - imu.getHeading(), 360);
        moveData.speed = abs(coordinatePID.update(difference.getMagnitude(), 0));
    }

    return false;
}

bool moveToCoordinate(Point point) {
    if (camera.goalsVisible()) {
        // If the robot position can be calculated, move by the difference between the coordinate and the robot position
        return moveByDifference(point - robotPosition);
    } else {
        // No robot position, stop moving
        moveData.angle = 0;
        moveData.speed = 0;
    }

    return false;
}

void goalTracking() {
    if (attackingGoalVisible()) {
        // If the attacking goal is visible, face it
        facingDirection = attackingGoalAngle;
        facingGoal = true;
    } else {
        // Otherwise face forward
        facingGoal = false;
    }
}

void updateCamera() {
    camera.update();

    if (camera.goalsVisible()) {
        if (camera.newData()) {
            // If the camera has new data, compute the robot position

            int angle = camera.yellowClosest() ? camera.yellowAngle : camera.blueAngle;
            double distance = camera.shortestDistance();

            angle = mod(angle + imu.getHeading(), 360);

            // Robot position calculated using the distance and angle to goal. Polar -> cartesian
            robotPosition.x = constrain(distance * -sin(degreesToRadians(angle)), -(FIELD_WIDTH_CENTIMETERS / 2), FIELD_WIDTH_CENTIMETERS / 2);
            robotPosition.y = (FIELD_LENGTH_CENTIMETERS / 2 - GOAL_EDGE_OFFSET_CENTIMETERS) * (settings.goalIsYellow && camera.yellowClosest() || !settings.goalIsYellow && !camera.yellowClosest() ? 1 : -1) + distance * -cos(degreesToRadians(angle));

            // Calculate absolute goal angles using IMU heading
            attackingGoalAngle = settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
            defendingGoalAngle = !settings.goalIsYellow ? mod(camera.yellowAngle + imu.getHeading(), 360) : mod(camera.blueAngle + imu.getHeading(), 360);
        }
    }
}

void avoidDoubleDefence() {
    if (bluetooth.isConnected) {
        // If connected to the other robot, avoid double defence
        int moveAngle = moveData.angle;
        int moveSpeed = moveData.speed;

        // Move back into the allowed area if outside. Can't go back further than a certain Y distance
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
        // Moving sideways when switching to defence to let defender go through to the other goal

        if ((ballData.visible() || bluetooth.otherData.ballData.visible()) && !bluetooth.otherData.ballIsOut) {
            // If the ball is still visible to both robots, move to the coordinate until there
            movingSideways = !moveToCoordinate(sidewaysCoordinate);
        } else {
            movingSideways = false;
        }

        facingGoal = false;
    } else if (bluetooth.otherData.ballIsOut) {
        // Other robot sees the ball out, move to an advantageous position in the centre

        moveToCoordinate(Point(BALL_OUT_CENTRE_X, BALL_OUT_CENTRE_Y));
        facingGoal = false;
    } else if (ballData.visible()) {
        // Track the goal and orbit the ball

        goalTracking();
        calculateOrbit();
    } else {
        // No ball, go to the centre of the field

        moveToCoordinate(Point(NO_BALL_CENTRE_X, NO_BALL_CENTRE_Y));
        facingGoal = false;
    }
}

void defend() {
    if (camera.goalsVisible()) {
        // Position of the defending goal
        Point goalPosition = Point(0, -(FIELD_LENGTH_CENTIMETERS / 2) + GOAL_EDGE_OFFSET_CENTIMETERS);

        if (ballData.visible()) {
            // Relative ball position
            Point relativeBallPosition = ballData.position(imu.getHeading());

            if (ballData.strength > DEFEND_CHARGE_STRENGTH && angleIsInside(360 - DEFEND_CHARGE_ANGLE, DEFEND_CHARGE_ANGLE, ballData.angle) && robotPosition.y < DEFEND_CHARGE_MAX_Y) {
                // Ball is within the capture zone, charge forward until the robot has reached a certain maxmimum Y coordinate
                moveData.speed = DEFEND_CHARGE_SPEED;
                moveData.angle = 0;
            } else if (relativeBallPosition.y < 0) {
                // Ball is behind the robot

                if (robotPosition.y < goalPosition.y + DEFEND_GOAL_DISTANCE_ORBIT) {
                    // Robot is too close to the goal to orbit, move to one side and go close to the goal
                    moveToCoordinate(Point(MAX_DEFEND_X * sign(relativeBallPosition.x), goalPosition.y + DEFEND_GOAL_DISTANCE_CLOSE));
                    facingDirection = mod(90 * sign(relativeBallPosition.x), 360);
                } else {
                    // Robot is far enough out, orbit behind the ball
                    calculateOrbit();
                }

                facingGoal = false;
            } else {
                // Position the robot inbetween the ball and the centre of the goal. Use PID to move left and right and coordinates to position a distance from the goal
                double ballAngle = mod(ballData.angle - (180 + defendingGoalAngle), 360);
                double xDifference = defendPID.update(smallestAngleBetween(ballAngle, 0) * -sign(ballAngle - 180), 0);

                moveByDifference(Point(abs(robotPosition.x) > MAX_DEFEND_X && sign(xDifference) == sign(robotPosition.x) ? MAX_DEFEND_X * sign(robotPosition.x) - robotPosition.x : xDifference, goalPosition.y + DEFEND_GOAL_DISTANCE - robotPosition.y));
            }
        } else {
            // No ball, move to the centre of the goal
            moveToCoordinate(Point(0, goalPosition.y + DEFEND_GOAL_DISTANCE));
            facingGoal = false;
        }
    } else {
        if (ballData.visible()) {
            // No goal, orbit like an attacker
            calculateOrbit();
            facingGoal = false;
        } else {
            // Nothing, stop
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
        // Rotation correction PID for goal
        moveData.rotation = (int8_t)round(goalHeadingAttackPID.update(doubleMod(doubleMod(imu.getHeading() - facingDirection, 360) + 180, 360) - 180, 0));
    } else {
        // Normal rotation correction PID
        moveData.rotation = (int8_t)round(headingPID.update(doubleMod(imu.getHeading() + 180, 360) - 180, 0));
    }
}

void updateDebug() {
    // Send and receive data from debug slave

    slaveDebug.updateDebugSettings();
    settings = slaveDebug.debugSettings;

    slaveDebug.sendPlayMode(currentPlayMode() == PlayMode::attackMode);
    slaveDebug.sendBluetoothConnected(bluetooth.isConnected);

    if (!settings.gameMode) {
        // Game mode, don't send any debugging data
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
        // Calibrate both IMU and light sensors

        // Stop motors
        slaveMotor.brake();

        imu.calibrate();
        imu.resetHeading();
        slaveSensor.sendCalibrateLightSensors();
        lineData = LineData();
        delay(500);

        slaveDebug.sendLightSensorsAreReset();
        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();

        // Wait to ensure debug slave recieves calibration finished before checking again

        delay(500);
    } else if (settings.IMUNeedsResetting) {
        // Calibrate IMU

        // Stop motors
        slaveMotor.brake();

        imu.calibrate();
        imu.resetHeading();

        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();

        // Wait to ensure debug slave recieves calibration finished before checking again

        delay(500);
    } else if (settings.lightSensorsNeedResetting) {
        // Calibrate light sensors

        // Stop motors
        slaveMotor.brake();

        slaveSensor.sendCalibrateLightSensors();
        lineData = LineData();

        delay(500);

        slaveDebug.sendLightSensorsAreReset();
        slaveDebug.updateDebugSettings();

        // Wait to ensure debug slave recieves calibration finished before checking again

        delay(500);
    }

    slaveDebug.updateDebugSettings();
}

bool shouldSwitchPlayMode(BluetoothData attackerData, BluetoothData defenderData) {
    // Switch play mode if conditions met

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
        // Undecided play mode, pick default unless the other robot has a play mode

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
        // Robot ID 0 is always the the opposite of the other robot
        playMode = bluetooth.otherData.playMode == PlayMode::attackMode ? PlayMode::defendMode : PlayMode::attackMode;
    }

    if (playMode != previousPlayMode && previousPlayMode == PlayMode::attackMode) {
        // If switched to defender, move to the side
        movingSideways = true;
        sidewaysCoordinate = Point(40 * sign(robotPosition.x), robotPosition.y);
    } else {
        movingSideways = false;
    }
}

void updateBluetooth() {
    if (settings.playModeSwitching) {
        // Send and recieve bluetooth data if playmode switching is enabled
        bluetoothData = BluetoothData(ballData, imu.getHeading(), isOutsideLine(ballData.angle), playMode, lineData.onField, robotPosition);
        bluetooth.update(bluetoothData);

        if (bluetooth.isConnected) {
            updatePlayMode();
        } else if (bluetooth.previouslyConnected) {
            // Defend if the other robot disconnects
            playMode = PlayMode::defendMode;
        }
    } else {
        bluetooth.disconnect();
    }
}

void setup() {
    #if WRITE_ROBOT_ID_EEPROM
        // Write robot ID to EEPROM if required
        EEPROM.write(ROBOT_ID_EEPROM_ADDRESS, ROBOT_ID_WRITE);
    #endif

    // Read robot ID
    robotID = EEPROM.read(ROBOT_ID_EEPROM_ADDRESS);

    // Default play mode from robot ID
    defaultPlayMode = robotID == 0 ? PlayMode::attackMode : PlayMode::defendMode;

    // Setup communications and sensors

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
    // Update sensor data
    slaveSensor.updateBallData();
    ballData = slaveSensor.ballData();

    slaveSensor.updateLineAngle();
    slaveSensor.updateLineSize();

    updateLine(slaveSensor.lineAngle, slaveSensor.lineSize);

    imu.update();

    #if CAMERA_ENABLED
        updateCamera();
    #endif

    // Bluetooth

    if (bluetoothTimer.timeHasPassed()) {
        updateBluetooth();
    }

    // Tactics and movement

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
