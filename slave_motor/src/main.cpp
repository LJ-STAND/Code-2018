#include <Motor.h>
#include <Arduino.h>
#include <Pins.h>
#include <PID.h>

Motor motorLeft(MOTOR_LEFT_PWM, MOTOR_LEFT_INA, MOTOR_LEFT_INB, MOTOR_LEFT_ENA, MOTOR_LEFT_ENB);
Motor motorRight(MOTOR_RIGHT_PWM, MOTOR_RIGHT_INA, MOTOR_RIGHT_INB, MOTOR_RIGHT_ENA, MOTOR_RIGHT_ENB);
Motor motorBackLeft(MOTOR_BACKLEFT_PWM, MOTOR_BACKLEFT_INA, MOTOR_BACKLEFT_INB, MOTOR_BACKLEFT_ENA, MOTOR_BACKLEFT_ENB);
Motor motorBackRight(MOTOR_BACKRIGHT_PWM, MOTOR_BACKRIGHT_INA, MOTOR_BACKRIGHT_INB, MOTOR_BACKRIGHT_ENA, MOTOR_BACKRIGHT_ENB);

void encoderLeftInterrupt() {
    motorLeft.encoderInterrupt();
}

void encoderRightInterrupt() {
    motorRight.encoderInterrupt();
}

void encoderBackLeftInterrupt() {
    motorBackLeft.encoderInterrupt();
}

void encoderBackRightInterrupt() {
    motorBackRight.encoderInterrupt();
}

void setup() {
    analogWriteResolution(16);

    attachInterrupt(ENCODER_L_A, encoderLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_R_A, encoderRightInterrupt, CHANGE);
    attachInterrupt(ENCODER_BL_A, encoderBackLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_BR_A, encoderBackRightInterrupt, CHANGE);
}

void loop() {

}

// #include <t3spi.h>
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
