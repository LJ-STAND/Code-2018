#include <Arduino.h>
#include <t3spi.h>
#include <TSOPArray.h>
#include <Config.h>
#include <Slave.h>
#include <LightSensorArray.h>
#include <Timer.h>

T3SPI spi;

TSOPArray tsops;
LightSensorArray lightSensors;

Timer ledTimer(LED_BLINK_TIME_SLAVE_SENSOR);
bool ledOn;

void setup() {
    Serial.begin(9600);

    spi.begin_SLAVE(SLAVE_SENSOR_SCK, SLAVE_SENSOR_MOSI, SLAVE_SENSOR_MISO, SLAVE_SENSOR_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    tsops.init();

    lightSensors.init();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    tsops.updateOnce();

    if (tsops.tsopCounter > TSOP_LOOP_COUNT) {
        tsops.finishRead();
        tsops.unlock();

        lightSensors.read();
        lightSensors.calculateClusters();
        lightSensors.calculateLine();
    }

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}

void spi0_isr() {
    uint16_t dataIn = SPI0_POPR;

    uint8_t command = (dataIn >> 10);
    uint16_t data = dataIn & 0x3FF;

    uint16_t sendData;

    switch (command) {
    case SlaveCommand::ballAngleCommand:
        sendData = (uint16_t)tsops.getAngle();
        break;

    case SlaveCommand::ballStrengthCommand:
        sendData = (uint16_t)tsops.getStrength();
        break;

    case SlaveCommand::lineAngleCommand:
        sendData = (uint16_t)lightSensors.getLineAngle();
        break;

    case SlaveCommand::lineSizeCommand:
        sendData = 
        (uint16_t)(lightSensors.getLineSize() * 100.0);
        break;

    case SlaveCommand::lsFirst16BitCommmand:
        sendData = 0;

        for (uint8_t i = 0; i < 16; i++) {
            sendData |= lightSensors.data[i] << i;
        }

        break;

    case SlaveCommand::lsSecond16BitCommand:
        sendData = 0;

        for (uint8_t i = 0; i < 16; i++) {
            sendData |= lightSensors.data[i + 16] << i;
        }

        break;

    case SlaveCommand::calibrateLightSensorsCommand:
        lightSensors.calibrate();

        break;

    default:
        sendData = 0;
        break;
    }

    SPI0_PUSHR_SLAVE = (command << 10) | (sendData & 0x3FF);
    SPI0_SR |= SPI_SR_RFDF;
}
