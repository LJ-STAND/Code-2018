#include <Arduino.h>
#include <Pins.h>
#include <MotorArray.h>
#include <t3spi.h>
#include <Timer.h>
#include <Slave.h>
#include <Config.h>

// Motor object
MotorArray motors;

// Movement data
volatile int8_t speed;
volatile uint16_t angle;
volatile int8_t rotation;

// SPI object
T3SPI spi;

// Teensy LED
Timer ledTimer = Timer(LED_BLINK_TIME_SLAVE_MOTOR);
bool ledOn;

void setup() {
    motors.init();

    spi.begin_SLAVE(SLAVE_MOTOR_SCK, SLAVE_MOTOR_MOSI, SLAVE_MOTOR_MISO, SLAVE_MOTOR_CS);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    motors.move(angle, rotation, speed);
    motors.update();
    
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
    case SlaveCommand::motorAngleCommand:
        angle = (uint16_t)data;
        break;

    case SlaveCommand::motorRotationCommand:
        rotation = (int8_t)data;
        break;

    case SlaveCommand::motorSpeedCommand:
        speed = (int8_t)data;
        break;

    case SlaveCommand::motorLeftRPMCommand:
        sendData = motors.motorLeft.getRPM();
        break;

    case SlaveCommand::motorRightRPMCommand:
        sendData = motors.motorRight.getRPM();
        break;

    case SlaveCommand::motorBackLeftRPMCommand:
        sendData = motors.motorBackLeft.getRPM();
        break;

    case SlaveCommand::motorBackRightRPMCommand:
        sendData = motors.motorBackRight.getRPM();
        break;
    }

    SPI0_PUSHR_SLAVE = (command << 10) | (sendData & 0x3FF);
    SPI0_SR |= SPI_SR_RFDF;
}