#include <Arduino.h>
#include <t3spi.h>
#include <TSOPArray.h>
#include <Config.h>
#include <Slave.h>
#include <LightSensorArray.h>
#include <Timer.h>

// SPI object
T3SPI spi;

// Sensors
TSOPArray tsops;
LightSensorArray lightSensors;

// Update IR sensors for a certain amount of time before computing ball data
Timer tsopTimer(833 * TSOP_TIMER_PERIOD_NUMBER);

// Teensy LED
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

    if (tsopTimer.timeHasPassed()) {
        // Once a certain amount of time has passed, compute ball data and update light sensors
        tsops.finishRead();

        lightSensors.read();
        lightSensors.calculateClusters();
        lightSensors.calculateLine();

        tsopTimer.resetTime();
    }

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}

void spi0_isr() {
    // SPI ISR. receive data and check command

    uint16_t dataIn = SPI0_POPR;

    // Split recieved data into command and data
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
        sendData = (uint16_t)(lightSensors.getLineSize() * 100.0);
        break;

    case SlaveCommand::lsFirstCommand:
        sendData = 0;

        for (uint8_t i = 0; i < 10; i++) {
            sendData |= lightSensors.data[i] << i;
        }

        break;

    case SlaveCommand::lsSecondCommand:
        sendData = 0;

        for (uint8_t i = 0; i < 10; i++) {
            sendData |= lightSensors.data[i + 10] << i;
        }

        break;

    case SlaveCommand::lsThirdCommand:
        sendData = 0;

        for (uint8_t i = 0; i < 10; i++) {
            sendData |= lightSensors.data[i + 20] << i;
        }

        break;

    case SlaveCommand::lsFourthCommand:
        sendData = 0;

        for (uint8_t i = 0; i < 6; i++) {
            sendData |= lightSensors.data[i + 30] << i;
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
