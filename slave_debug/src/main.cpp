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
#include <Timer.h>
#include <DebugController.h>

T3SPI spi;
volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

volatile int ballAngle = 400;
volatile int ballStrength;

LED leds;
Screen screen;

Timer ledTimer(LED_BLINK_TIME_SLAVE_DEBUG);
bool ledOn;

DebugController debug;

void setup(void) {
    Serial.begin(9600);
    Serial5.begin(9600);

    screen.init();

    debug.init();

    spi.begin_SLAVE(SLAVE_DEBUG_SCK, SLAVE_DEBUG_MOSI, SLAVE_DEBUG_MISO, SLAVE_DEBUG_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    leds.init();

    pinMode(LED_BUILTIN, OUTPUT);
}

bool on = false;
bool canPress = true;

void loop() {
    if (screen.settings.IMUNeedsCalibrating) {
        leds.rainbow();
    } else {
        if (ballAngle != 400) {
            leds.displayAngle(ballAngle, 300);
            debug.appSendBallAngle(ballAngle);
            delay(100);
        } else {
            leds.rgbColor(leds.rgb.Color(100, 0, 0));
        }
    }

    screen.update();

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
    case SlaveCommand::lsFirst16BitCommmand:
        break;

    case SlaveCommand::lsSecond16BitCommand:
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

    case SlaveCommand::debugSettingsCommand:
        dataOut[0] = screen.settings.numberValue();
        break;

    case SlaveCommand::headingIsResetCommand:
        screen.settings.headingNeedsResetting = false;
        break;

    case SlaveCommand::IMUIsCalibratedCommand:
        screen.settings.IMUNeedsCalibrating = false;
        screen.clearMessage();
        break;

    case SlaveCommand::headingCommand:
        screen.heading = data;
        break;

    case SlaveCommand::motorLeftRPMCommand:
        screen.leftRPM = data * 2;
        break;

    case SlaveCommand::motorRightRPMCommand:
        screen.rightRPM = data * 2;
        break;

    case SlaveCommand::motorBackLeftRPMCommand:
        screen.backLeftRPM = data * 2;
        break;

    case SlaveCommand::motorBackRightRPMCommand:
        screen.backRightRPM = data * 2;
        break;
    }
}
