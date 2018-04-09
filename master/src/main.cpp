#include <t3spi.h>
#include <Arduino.h>

//The number of integers per data packet
#define dataLength 256

//Initialize the arrays for outgoing data
//volatile uint8_t  data[dataLength] = {};
volatile uint16_t data[dataLength] = {69};


//Initialize the arrays for incoming data
//volatile uint8_t returnData[dataLength] = {};
volatile uint16_t returnData[dataLength] = {};


T3SPI spi;

void setup(){
	spi = T3SPI();

	Serial.begin(9600);

	//Begin SPI in MASTER (SCK pin, MOSI pin, MISO pin, CS pin, Active State)
	spi.begin_MASTER(ALT_SCK, MOSI, MISO, CS0, CS_ActiveLOW);

	//Set the CTAR (CTARn, Frame Size, SPI Mode, Shift Mode, Clock Divider)
	//spi.setCTAR(CTAR0,8,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV2);
	spi.setCTAR(CTAR_0,16,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV32);

	//Populate data array
	for (int i=0; i<dataLength; i++){
		data[i]=i;
	}

	//Wait for Slave
	delay(5000);
}

void loop(){

	//Capture the time before sending data
	spi.timeStamp1 = micros();

	//Send n number of data packets
	for (int i=0; i<1; i++) {

		//Send data (data array, data array length, CTARn, CS pin)
		//spi.txrx8(data, returnData, dataLength,CTAR0,CS1);}
		spi.txrx16(data, returnData, dataLength,CTAR_0,CS0);}

		//Capture the time when transfer is done
		spi.timeStamp2 = micros();

		//Print data sent & data received
		for (int i=0; i<dataLength; i++){
			Serial.print("data[");
			Serial.print(i);
			Serial.print("]: ");
			Serial.print(data[i]);
			Serial.print("   returnData[");
			Serial.print(i);
			Serial.print("]: ");
			Serial.println(returnData[i]);
			Serial.flush();}

			//Print statistics for the previous transfer
			spi.printStatistics(dataLength);

			//Reset the packet count
			spi.packetCT=0;

			delay(2000);
		}
