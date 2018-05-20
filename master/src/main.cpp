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

T3SPI spi;

SlaveSensor slaveSensor;
SlaveMotor slaveMotor;
SlaveDebug slaveDebug;

Bluetooth bluetooth;

IMUFusion imu;
Camera camera;

DebugSettings settings;

LineData lineData(0, 0, true);
BallData ballData;
MoveData moveData;

Timer ledTimer(LED_BLINK_TIME_MASTER);
Timer slaveDebugUpdateTimer(SLAVE_DEBUG_UPDATE_TIME);

PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);

bool ledOn;

void setup() {
    Serial.begin(9600);

    bluetooth.init();

    spi.begin_MASTER(MASTER_SCK, MASTER_MOSI, MASTER_MISO, MASTER_CS_MOTOR, CS_ActiveLOW);
    spi.setCTAR(CTAR_0, 16, SPI_MODE0, LSB_FIRST, SPI_CLOCK_DIV16);

    slaveSensor.init();
    slaveMotor.init();
    slaveDebug.init();

    imu.init();
    camera.init();

	pinMode(LED_BUILTIN, OUTPUT);
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
    moveData.speed = ballData.visible() ? ORBIT_SPEED : 0;

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
}

void updateCamera() {

}

void calculateMovement() {
    // if (currentPlayMode() == PlayMode::attack) {
    //     if (xbee.otherBallIsOut) {
    //         attackingBackwards = false;
    //         centre(CENTRE_GOAL_DISTANCE_CLOSE);
    //     } else {
    //         if (attackingBackwards) {
    //             ballData.angle = mod(ballData.angle + 180, 360);
    //
    //             calculateOrbit();
    //             moveData.angle = mod(moveData.angle + 180, 360);
    //
    //             if (!ballData.visible) {
    //                 attackingBackwards = false;
    //             } else {
    //                 if (smallestAngleBetween(imu.heading, 0) < 90) {
    //                     attackingBackwards = false;
    //                 } else if (!lineData.onField && smallestAngleBetween(lineData.angle, ballData.angle) < 90) {
    //                     attackingBackwards = false;
    //                 } else if (goalData.status != GoalStatus::invisible) {
    //                     if (switchingStrengthAverage.average() < ATTACK_BACKWARDS_MAX_STRENGTH && (goalData.distance < DEFEND_LEFT_GOAL_DISTANCE)) {
    //                         attackingBackwards = false;
    //                     }
    //                 } else {
    //                     if (switchingStrengthAverage.average() < ATTACK_BACKWARDS_MAX_STRENGTH) {
    //                         attackingBackwards = false;
    //                     }
    //                 }
    //             }
    //         } else {
    //             if (ballData.visible) {
    //                 calculateOrbit();
    //             } else {
    //                 centre(CENTRE_GOAL_DISTANCE);
    //             }
    //         }
    //     }
    // } else {
    //     calculateDefense();
    // }

    calculateOrbit();
    // calculateLineAvoid();

    moveData.rotation = (int8_t)round(headingPID.update(doubleMod(imu.getHeading() + 180, 360) - 180, 0));
}

void updateDebug() {
    slaveDebug.updateDebugSettings();
    settings = slaveDebug.debugSettings;

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

        slaveDebug.sendLineData(lineData);
    }    
    
    if (settings.IMUNeedsResetting) {
        imu.calibrate();
        imu.resetHeading();
        slaveDebug.sendIMUIsReset();
        slaveDebug.updateDebugSettings();
        delay(500);
    }

    if (settings.lightSensorsNeedResetting) {
        slaveSensor.sendCalibrateLightSensors();
        lineData = LineData();
        delay(500);
        slaveDebug.sendLightSensorsAreReset();
        slaveDebug.updateDebugSettings();
        delay(500);
    }

    slaveDebug.updateDebugSettings();
}

void loop() {
    slaveSensor.updateBallData();
    slaveSensor.updateLineAngle();
    slaveSensor.updateLineSize();
    
    ballData = slaveSensor.ballData();
    
    updateLine(slaveSensor.lineAngle, slaveSensor.lineSize);

    imu.update();
    camera.update();

    if (settings.engineStarted) {
        calculateMovement();

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
