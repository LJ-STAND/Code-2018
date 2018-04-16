#include <Arduino.h>
#include <t3spi.h>
#include <Config.h>
#include <Pins.h>
#include <Slave.h>
#include <Common.h>
#include <BallData.h>
#include <IMU.h>
#include <SPI.h>
#include <Timer.h>

typedef struct MoveData {
    int angle = 0, speed = 0, rotation = 0;

    MoveData() {}
    MoveData(int moveAngle, int moveSpeed, int moveRotation) {
        angle = moveAngle;
        speed = moveSpeed;
        rotation = moveRotation;
    }
} MoveData;

SlaveTSOP slaveTSOP;

BallData ballData;
MoveData moveData;

T3SPI spi;

Timer ledTimer(LED_BLINK_TIME_MASTER);

IMUFusion imu;

void setup() {

    delay(2000);

	spi = T3SPI();

    // Serial
    Serial.begin(9600);

    // SPI
    spi.begin_MASTER(ALT_SCK, MOSI, MISO, CS1, CS_ActiveLOW);
    spi.setCTAR(CTAR_0, 16, SPI_MODE0, LSB_FIRST, SPI_CLOCK_DIV32);
	spi.enableCS(CS0, CS_ActiveLOW);

    slaveTSOP.init();

	pinMode(13, OUTPUT);

    Serial5.begin(9600);

    pinMode(INT_PIN, INPUT);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	imu.init();
}

void calculateOrbit() {
    moveData.speed = ORBIT_SPEED;

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

void loop() {
    ballData = slaveTSOP.getBallData();
	calculateOrbit();

    dataOut[0] = (uint16_t)heading;
    spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
    spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
    spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);


	dataOut[0] = 0x8000 | (uint16_t)moveData.speed;
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);

	dataOut[0] = 0x4000 | (uint16_t)moveData.angle;
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);

	digitalWrite(13, HIGH);

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}
