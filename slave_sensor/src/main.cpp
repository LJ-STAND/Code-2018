#include <Arduino.h>
#include <t3spi.h>
#include <TSOPArray.h>
#include <Config.h>
#include <Slave.h>
#include <LightSensorArray.h>

T3SPI spi;

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
    // for (int i = 0; i < LS_NUM; i++) {
    //     Serial.print(lightSensors.data[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();


    lightSensors.calculateClusters();
    lightSensors.calculateLine();

    // Serial.println(lightSensors.getLineAngle());
    //
    // tsops.updateOnce();
    //
    // if (tsops.tsopCounter > TSOP_LOOP_COUNT) {
    //     tsops.finishRead();
    //     tsops.unlock();
    // }
}

void spi0_isr() {
    uint16_t dataIn = SPI0_POPR;

    uint8_t command = (dataIn >> 10);
    uint16_t data = dataIn & 0x3FF;

    uint16_t dataOut = 0;

    switch (command) {
    case SlaveCommand::ballAngleCommand:
        dataOut = (uint16_t)tsops.getAngle();
        break;

    case SlaveCommand::ballStrengthCommand:
        dataOut = (uint16_t)tsops.getStrength();
        break;

    default:
        dataOut = 0;
        break;
    }

    Serial.println(dataOut);

    SPI0_PUSHR_SLAVE = dataOut;
    SPI0_SR |= SPI_SR_RFDF;
}
