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
