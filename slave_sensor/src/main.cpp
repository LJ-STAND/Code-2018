#include <Arduino.h>
#include <t3spi.h>
#include <TSOPArray.h>
#include <Config.h>
#include <Slave.h>
#include <LightSensorArray.h>
#include <Timer.h>

T3SPI spi;

volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

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
    lightSensors.calibrate();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    tsops.updateOnce();

    if (tsops.tsopCounter > TSOP_LOOP_COUNT) {
        tsops.finishRead();
        tsops.unlock();
        Serial.println(tsops.getStrength());

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
    spi.rxtx16(dataIn, dataOut, 1);

    uint8_t command = (dataIn[0] >> 10);
    uint16_t data = dataIn[0] & 0x3FF;

    switch (command) {
    case SlaveCommand::ballAngleCommand:
        dataOut[0] = (uint16_t)tsops.getAngle();
        break;

    case SlaveCommand::ballStrengthCommand:
        dataOut[0] = (uint16_t)tsops.getStrength();
        break;

    case SlaveCommand::lineAngleCommand:
        dataOut[0] = (uint16_t)lightSensors.getLineAngle();
        break;

    case SlaveCommand::lineSizeCommand:
        dataOut[0] = (uint16_t)(lightSensors.getLineSize() * 100.0);
        break;

    case SlaveCommand::lsFirst16BitCommmand:
        dataOut[0] = 0;

        for (uint8_t i = 0; i < 16; i++) {
            dataOut[0] |= lightSensors.data[i] << i;
        }

        break;

    case SlaveCommand::lsSecond16BitCommand:
        dataOut[0] = 0;

        for (uint8_t i = 0; i < 16; i++) {
            dataOut[0] |= lightSensors.data[i + 16] << i;
        }

        break;

    default:
        dataOut[0] = 0;
        break;
    }
}
