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

BallData ballData = BallData(TSOP_NO_BALL, 0);
LineData lineData = LineData();

int yellowAngle, yellowDistance, blueAngle, blueDistance;
DebugController debug = DebugController();

LED leds;
Screen screen;

Timer ledTimer(LED_BLINK_TIME_SLAVE_DEBUG);
bool ledOn;

Timer appDebugTimer(50);

void setup(void) {
    Serial.begin(9600);
    Serial5.begin(9600);

    screen.init();

    spi.begin_SLAVE(SLAVE_DEBUG_SCK, SLAVE_DEBUG_MOSI, SLAVE_DEBUG_MISO, SLAVE_DEBUG_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    leds.init();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (screen.settings.gameMode) {
        leds.rgbOff();
    } else {
        if (screen.settings.IMUNeedsResetting || screen.settings.lightSensorsNeedResetting) {
            leds.rainbow();
        } else {
            switch (screen.rgbType) {
            case RGBType::ballRGBType:
                if (ballData.visible()) {
                    leds.displayAngleSize(ballData.angle, ballData.strength, BALL_VIEW_MAX_STRENGTH, 180, 360);
                } else {
                    leds.rgbColor(leds.rgb.Color(100, 0, 0));
                }

                break;

            case RGBType::lineRGBType:
                if (lineData.onField) {
                    leds.rgbColor(leds.rgb.Color(0, 100, 0));
                } else {
                    leds.displayAngleSize(lineData.angle, lineData.size, 3, 120, 0);
                }

                break;

            case RGBType::rainbowRGBType:
                leds.rainbow();

                break;

            case RGBType::goalRGBType:
                if (screen.settings.goalIsYellow) {
                    if (yellowAngle != NO_GOAL_ANGLE) {
                        leds.displayAngle(yellowAngle, 60);
                    } else {
                        leds.rgbColor(leds.rgb.Color(100, 0, 0));
                    }
                } else {
                    if (blueAngle != NO_GOAL_ANGLE) {
                        leds.displayAngle(blueAngle, 240);
                    } else {
                        leds.rgbColor(leds.rgb.Color(100, 0, 0));
                    }                    
                }
            
                break;
            }
        }
    }

    screen.update();

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }

    if (appDebugTimer.timeHasPassed()) {
        debug.appSendBallAngle(ballData.angle);
        debug.appSendBallStrength(ballData.strength);
        debug.appSendLineAngle(lineData.angle);
        debug.appSendLineSize(lineData.size);
    }
}

void spi0_isr() {
    uint16_t dataIn = SPI0_POPR;

    uint8_t command = (dataIn >> 10);
    uint16_t data = dataIn & 0x3FF;

    uint16_t sendData;

    switch (command) {
    case SlaveCommand::lsFirstCommand:
        screen.lsFirst = data;
        break;

    case SlaveCommand::lsSecondCommand:
        screen.lsSecond = data;
        break;

    case SlaveCommand::lsThirdCommand:
        screen.lsThird = data;
        break;

    case SlaveCommand::lsFourthCommand:
        screen.lsFourth = data;
        break;

    case SlaveCommand::playModeCommand:
        leds.displayPlayMode(data);
        break;

    case SlaveCommand::ballAngleCommand:
        ballData.angle = data;
        screen.ballData = ballData;
        break;

    case SlaveCommand::ballStrengthCommand:
        ballData.strength = data;
        screen.ballData = ballData;
        break;

    case SlaveCommand::debugSettingsCommand:
        sendData = screen.settings.numberValue();
        break;

    case SlaveCommand::IMUIsResetCommand:
        screen.settings.IMUNeedsResetting = false;
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

    case SlaveCommand::lightSensorsAreResetCommand:
        screen.settings.lightSensorsNeedResetting = false;
        screen.clearMessage();

        break;

    case SlaveCommand::debugTerminalCommand:
        screen.write(data);
        
        break;

    case SlaveCommand::lineAngleCommand:
        lineData.angle = data;
        screen.lineData = lineData;
        
        break;

    case SlaveCommand::lineSizeCommand:
        lineData.size = (double)data / 100.0;
        screen.lineData = lineData;

        break;

    case SlaveCommand::lineOnFieldCommand:
        lineData.onField = data;
        screen.lineData = lineData;

        break;

    case SlaveCommand::yellowAngleCommand:
        yellowAngle = data;
        screen.yellowAngle = yellowAngle;

        break;

    case SlaveCommand::yellowDistanceCommand:
        yellowDistance = data;
        screen.yellowDistance = yellowDistance;

        break;

    case SlaveCommand::blueAngleCommand:
        blueAngle = data;
        screen.blueAngle = blueAngle;

        break;

    case SlaveCommand::blueDistanceCommand:
        blueDistance = data;
        screen.blueDistance = blueDistance;

        break;

    case SlaveCommand::robotPositionXCommand:
        screen.robotPositionX = (int8_t)data;
        break;

    case SlaveCommand::robotPositionYCommand:
        screen.robotPositionY = (int8_t)data;
        break;

    case SlaveCommand::bluetoothConnectedCommand:
        screen.setBluetoothConnected(data);
        break;
    }

    SPI0_PUSHR_SLAVE = (command << 10) | (sendData & 0x3FF);
    SPI0_SR |= SPI_SR_RFDF;
}
