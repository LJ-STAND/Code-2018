#ifndef SLAVE_H
#define SLAVE_H

#include <t3spi.h>
#include <Pins.h>
#include <Config.h>

enum SlaveCommand: uint8_t {
    motorAngleCommand,
    motorRotationCommand,
    motorSpeedCommand,
    ballAngleCommand,
    ballStrengthCommand,
    lineAngleCommand,
    lineSizeCommand
};

class Slave {
public:
    void init(int csPin);
    uint16_t transaction(SlaveCommand command, uint16_t data = 0);

private:
    volatile uint16_t dataIn[1];
    volatile uint16_t dataOut[1];
    int cs;
};

class SlaveMotor: public Slave {
public:
    void init();
    void setMotorAngle(uint16_t angle);
    void setMotorRotation(uint16_t rotation);
    void setMotorSpeed(uint16_t speed);
};

class SlaveSensor: public Slave {
public:
    void init();
    uint16_t getBallAngle();
    uint16_t getBallStrength();
    uint16_t getLineAngle();
    uint16_t getLineSize();
};

#endif // SLAVE_H
