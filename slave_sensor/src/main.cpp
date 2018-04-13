/*
 * SLAVE_TSOP
 *
 * Source file for the TSOP slave teensy for LJ STAND 2017
 */

#include <Arduino.h>
#include <t3spi.h>
#include <TSOPArray.h>
#include <Config.h>
#include <Slave.h>
#include <LightSensorArray.h>

T3SPI spi;

volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

TSOPArray tsops;
LightSensorArray lightSensors;

void setup() {
    Serial.begin(9600);

    spi.begin_SLAVE(SLAVE_SENSOR_SCLK, SLAVE_SENSOR_MOSI, SLAVE_SENSOR_MISO, SLAVE_SENSOR_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    tsops.init();
    lightSensors.init();

    delay(5000);
    lightSensors.calibrate();
}

void loop() {
    lightSensors.read();
    lightSensors.calculateClusters();
    lightSensors.calculateLine();

    for (int i = 0; i < LS_NUM; i++) {
        Serial.print(lightSensors.data[i]);
        Serial.print(", ");
    }

    Serial.println(String(lightSensors.getLineAngle()) + ", " + String(lightSensors.getLineSize()));

    tsops.updateOnce();

    if (tsops.tsopCounter > TSOP_LOOP_COUNT) {
        tsops.finishRead();
        tsops.unlock();
    }
}

void spi0_isr() {
    spi.rxtx16(dataIn, dataOut, 1);
    int command = dataIn[0];

    switch (command) {
        case SlaveCommand::tsopAngle:
            dataOut[0] = (uint16_t)tsops.getStrength();
            break;

        case SlaveCommand::tsopStrength:
            dataOut[0] = (uint16_t)tsops.getAngle();
            break;

        default:
            dataOut[0] = 0;
            break;
    }
}
//
// #include <t3spi.h>
// #include <Arduino.h>
//
// //Initialize T3SPI class as spi
// T3SPI spi;
//
// //The number of integers per data packet
// //MUST be the same as defined on the MASTER device
// #define dataLength 256
//
// //Initialize the arrays for incoming data
// //volatile uint8_t data[dataLength] = {};
// volatile uint16_t data[dataLength] = {};
//
// //Initialize the arrays for outgoing data
// //volatile uint8_t returnData[dataLength] = {};
// volatile uint16_t returnData[dataLength] = {};
//
//
// void setup(){
// spi = T3SPI();
//
//   Serial.begin(9600);
//
//   //Begin SPI in SLAVE (SCK pin, MOSI pin, MISO pin, CS pin)
//   spi.begin_SLAVE(ALT_SCK, MOSI, MISO, CS0);
//
//   //Set the CTAR0_SLAVE0 (Frame Size, SPI Mode)
//   //spi.setCTAR_SLAVE(8, SPI_MODE0);
//   spi.setCTAR_SLAVE(16, SPI_MODE0);
//
//   //Enable the SPI0 Interrupt
//   NVIC_ENABLE_IRQ(IRQ_SPI0);
//
//   //Poputlate the array of outgoing data
//   for (int i=0; i<dataLength; i++){
//     returnData[i]=i;}
// }
//
// void loop(){
//
//   //Capture the time before receiving data
//   if (spi.dataPointer==0 && spi.packetCT==0){
//     spi.timeStamp1=micros();}
//
//   //Capture the time when transfer is done
//   if (spi.packetCT==1){
//     spi.timeStamp2=micros();
//
//     //Print data received & data sent
//     for (int i=0; i<dataLength; i++){
//       Serial.print("data[");
//       Serial.print(i);
//       Serial.print("]: ");
//       Serial.print(data[i]);
//       Serial.print("   returnData[");
//       Serial.print(i);
//       Serial.print("]: ");
//       Serial.println(returnData[i]);
//       Serial.flush();}
//
//       //Print statistics for the previous transfer
//       spi.printStatistics(dataLength);
//
//     //Reset the packet count
//     spi.packetCT=0;}
//
// }
//
// //Interrupt Service Routine to handle incoming data
// void spi0_isr(void){
//
//   //Function to handle data
//   //spi.rxtx8 (data, returnData, dataLength);
//   spi.rxtx16(data, returnData, dataLength);
// }
