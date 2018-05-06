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

SlaveSensor slaveSensor;
SlaveMotor slaveMotor;

int ballAngle, ballStrength;

uint16_t motorAngle;
int8_t motorRotation = 0;
int8_t motorSpeed = 0;

T3SPI spi;

Timer ledTimer(LED_BLINK_TIME_MASTER);

IMUFusion imu;

PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);

bool ledOn;

void setup() {
    Serial.begin(9600);

	spi = T3SPI();
    spi.begin_MASTER(ALT_SCK, MOSI, MISO, CS1, CS_ActiveLOW);
    spi.setCTAR(CTAR_0, 16, SPI_MODE0, LSB_FIRST, SPI_CLOCK_DIV32);
	spi.enableCS(CS0, CS_ActiveLOW);

    slaveSensor.init();
    slaveMotor.init();

    imu.init();

	pinMode(LED_BUILTIN, OUTPUT);
}

void calculateOrbit() {
    motorSpeed = ballAngle == 400 ? 0 : ORBIT_SPEED;

    if (angleIsInside(360 - ORBIT_SMALL_ANGLE, ORBIT_SMALL_ANGLE, ballAngle)) {
        motorAngle = (int)round(ballAngle < 180 ? (ballAngle * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) : (360 - (360 - ballAngle) * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER));
    } else if (angleIsInside(360 - ORBIT_BIG_ANGLE, ORBIT_BIG_ANGLE, ballAngle)) {
        if (ballAngle < 180) {
            double nearFactor = (double)(ballAngle - ORBIT_SMALL_ANGLE) / (double)(ORBIT_BIG_ANGLE - ORBIT_SMALL_ANGLE);
            motorAngle = (int)round(90 * nearFactor + ballAngle * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER + ballAngle * (1 - ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) * nearFactor);
        } else {
            double nearFactor = (double)(360 - ballAngle - ORBIT_SMALL_ANGLE) / (double)(ORBIT_BIG_ANGLE - ORBIT_SMALL_ANGLE);
            motorAngle = (int)round(360 - (90 * nearFactor + (360 - ballAngle) * ORBIT_BALL_FORWARD_ANGLE_TIGHTENER + (360 - ballAngle) * (1 - ORBIT_BALL_FORWARD_ANGLE_TIGHTENER) * nearFactor));
        }
    } else {
        if (ballStrength > ORBIT_SHORT_STRENGTH) {
            motorAngle = ballAngle + (ballAngle < 180 ? 90 : -90);
        } else if (ballStrength > ORBIT_BIG_STRENGTH) {
            double strengthFactor = (double)(ballStrength - ORBIT_BIG_STRENGTH) / (double)(ORBIT_SHORT_STRENGTH - ORBIT_BIG_STRENGTH);
            double angleFactor = strengthFactor * 90;
            motorAngle = ballAngle + (ballAngle < 180 ? angleFactor : -angleFactor);
        } else {
            motorAngle = ballAngle;
        }
    }
}

void loop() {
    ballAngle = slaveSensor.getBallAngle();
    ballStrength = slaveSensor.getBallStrength();

    // Serial.println("ballAngle " + String(ballAngle));
    // Serial.println("ballStrength " + String(ballStrength));
    // delay(1000);
    // ballStrength = slaveSensor.getBallStrength();

	// calculateOrbit();
    //
    // imu.update();
    //
    // motorRotation = (int8_t)round(headingPID.update(doubleMod(imu.getHeading() + 180, 360) - 180, 0));
    //
    // slaveMotor.setMotorAngle(motorAngle);
    // slaveMotor.setMotorRotation(motorRotation);
    // slaveMotor.setMotorSpeed(motorSpeed);
    //
    // if (ledTimer.timeHasPassed()) {
    //     digitalWrite(LED_BUILTIN, ledOn);
    //     ledOn = !ledOn;
    // }
}
