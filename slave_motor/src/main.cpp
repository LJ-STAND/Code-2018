#include <Arduino.h>
#include <Pins.h>
#include <MotorArray.h>
#include <t3spi.h>
#include <Timer.h>
#include <Slave.h>
#include <Config.h>

MotorArray motors;

volatile int8_t speed;
volatile uint16_t angle;
volatile int8_t rotation;

T3SPI spi;

Timer ledTimer = Timer(LED_BLINK_TIME_SLAVE_MOTOR);
bool ledOn;

void encoderLeftInterrupt() {
    motors.motorLeft.updateEncoderCounts();
}

void encoderRightInterrupt() {
    motors.motorRight.updateEncoderCounts();
}

void encoderBackLeftInterrupt() {
    motors.motorBackLeft.updateEncoderCounts();
}

void encoderBackRightInterrupt() {
    motors.motorBackRight.updateEncoderCounts();
}

void setup() {
    motors.init();

    spi.begin_SLAVE(SLAVE_MOTOR_SCK, SLAVE_MOTOR_MOSI, SLAVE_MOTOR_MISO, SLAVE_MOTOR_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);

    attachInterrupt(ENCODER_L_A, encoderLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_L_B, encoderLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_R_A, encoderRightInterrupt, CHANGE);
    attachInterrupt(ENCODER_R_B, encoderRightInterrupt, CHANGE);
    attachInterrupt(ENCODER_BL_A, encoderBackLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_BL_B, encoderBackLeftInterrupt, CHANGE);
    attachInterrupt(ENCODER_BR_A, encoderBackRightInterrupt, CHANGE);
    attachInterrupt(ENCODER_BR_B, encoderBackRightInterrupt, CHANGE);
}

void loop() {
    // motors.move(angle, rotation, speed);
    // motors.update();

    motors.move(0, 0, 100);
    motors.motorLeft.update();

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}

void spi0_isr() {
    uint16_t dataIn = SPI0_POPR;

    uint8_t command = (dataIn >> 10);
    uint16_t data = dataIn & 0x3FF;

    uint16_t dataOut = 0;

    switch (command) {
    case SlaveCommand::motorAngleCommand:
        angle = (uint16_t)data;
        break;

    case SlaveCommand::motorRotationCommand:
        rotation = (int8_t)data;
        break;

    case SlaveCommand::motorSpeedCommand:
        speed = (int8_t)data;
        break;
    }

    SPI0_PUSHR_SLAVE = dataOut;
    SPI0_SR |= SPI_SR_RFDF;
}
