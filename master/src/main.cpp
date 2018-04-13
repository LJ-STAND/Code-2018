/*
* MASTER
*
* Source file for the master teensy for LJ STAND 2017
*/

#include <Arduino.h>
#include <t3spi.h>
#include <Config.h>
#include <Pins.h>
#include <Slave.h>
#include <Common.h>
#include <BallData.h>

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

volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

void setup() {

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

	Serial.println(String(ballData.strength) + ", " + String(ballData.angle));

	dataOut[0] = (0x1 << 15) | (uint16_t)moveData.speed;
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);

	dataOut[0] = (uint16_t)moveData.angle;
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);
	spi.txrx16(dataOut, dataIn, 1, CTAR_0, CS0);

	digitalWrite(13, HIGH);

    // while (Serial.available()) {
    //     uint8_t data = Serial.read();
    //     Serial5.write(data);
    //     Serial.write(data);
    //     Serial5.println();
    // }
}

// #include <t3spi.h>
// #include <Arduino.h>
//
// //The number of integers per data packet
// #define dataLength 256
//
// //Initialize the arrays for outgoing data
// //volatile uint8_t  data[dataLength] = {};
// volatile uint16_t data[dataLength] = {69};
//
//
// //Initialize the arrays for incoming data
// //volatile uint8_t returnData[dataLength] = {};
// volatile uint16_t returnData[dataLength] = {};
//
//
// T3SPI spi;
//
// void setup(){
// 	spi = T3SPI();
//
// 	Serial.begin(9600);
//
// 	//Begin SPI in MASTER (SCK pin, MOSI pin, MISO pin, CS pin, Active State)
// 	spi.begin_MASTER(ALT_SCK, MOSI, MISO, CS1, CS_ActiveLOW);
//
// 	//Set the CTAR (CTARn, Frame Size, SPI Mode, Shift Mode, Clock Divider)
// 	//spi.setCTAR(CTAR0,8,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV2);
// 	spi.setCTAR(CTAR_0,16,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV32);
//
// 	//Populate data array
// 	for (int i=0; i<dataLength; i++) {
// 		data[i]=i;
// 	}
//
// 	//Wait for Slave
// 	delay(5000);
// }
//
// void loop(){
//
// 	//Capture the time before sending data
// 	spi.timeStamp1 = micros();
//
// 	//Send n number of data packets
// 	for (int i=0; i<1; i++) {
//
// 		//Send data (data array, data array length, CTARn, CS pin)
// 		//spi.txrx8(data, returnData, dataLength,CTAR0,CS1);}
// 		spi.txrx16(data, returnData, dataLength,CTAR_0,CS1);
// 	}
//
// 		//Capture the time when transfer is done
// 		spi.timeStamp2 = micros();
//
// 		//Print data sent & data received
// 		for (int i=0; i<dataLength; i++){
// 			Serial.print("data[");
// 			Serial.print(i);
// 			Serial.print("]: ");
// 			Serial.print(data[i]);
// 			Serial.print("   returnData[");
// 			Serial.print(i);
// 			Serial.print("]: ");
// 			Serial.println(returnData[i]);
// 			Serial.flush();
// 		}
//
// 	//Print statistics for the previous transfer
// 	spi.printStatistics(dataLength);
//
// 	//Reset the packet count
// 	spi.packetCT=0;
//
// 	delay(2000);
// }
