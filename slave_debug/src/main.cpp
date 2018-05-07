#include <Arduino.h>
#include <Pins.h>
#include <Adafruit_ILI9341_8bit.h>
#include <Screen.h>
#include <MovingAverage.h>
#include <Config.h>
#include <t3spi.h>
#include <SPI.h>
#include <LED.h>
#include <Slave.h>
#include <DebugController.h>

T3SPI spi;
volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

volatile int ballAngle;
volatile int ballStrength;

uint8_t lsFirstByte;
uint8_t lsSecondByte;
uint8_t lsThirdByte;
uint8_t lsFourthByte;

LED leds;
Screen screen;

DebugController debug;

void setup(void) {

    Serial.begin(9600);
    Serial5.begin(9600);

    screen.init();

    spi.begin_SLAVE(SLAVE_DEBUG_SCK, SLAVE_DEBUG_MOSI, SLAVE_DEBUG_MISO, SLAVE_DEBUG_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    leds.init();
}

bool on = false;
bool canPress = true;

void loop() {
    if (ballAngle != 400) {
        leds.displayAngle(ballAngle, 300);
    } else {
        leds.rgbColor(leds.rgb.Color(100, 0, 0));
    }

    screen.updateBatteryMeter();
}

void spi0_isr() {
    spi.rxtx16(dataIn, dataOut, 1);

    uint8_t command = (dataIn[0] >> 10);
    uint16_t data = dataIn[0] & 0x3FF;

    switch (command) {
    case SlaveCommand::lsFirstByteCommmand:
        lsFirstByte = data;
        break;

    case SlaveCommand::lsSecondByteCommand:
        lsSecondByte = data;
        break;

    case SlaveCommand::lsThirdByteCommand:
        lsThirdByte = data;
        break;

    case SlaveCommand:: lsFourthByteCommand:
        lsFourthByte = data;
        break;

    case SlaveCommand::playModeCommand:
        leds.displayPlayMode((bool)data);
        break;
    case SlaveCommand::ballAngleCommand:
        ballAngle = data;
        break;
    case SlaveCommand::ballStrengthCommand:
        ballStrength = data;
        break;
    }
}
