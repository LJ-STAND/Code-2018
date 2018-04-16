#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

uint16_t Slave::transaction(SlaveCommand command, uint16_t data) {
    dataOut[0] = (command << 10) | (data & 0x3FF);

    spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);

    return dataIn[0];
}

void SlaveMotor::init() {
    Slave::init(MASTER_CS_MOTOR);
}

void SlaveMotor::setMotorAngle(uint16_t angle) {
    transaction(SlaveCommand::motorAngle, angle);
}

void SlaveMotor::setMotorRotation(uint16_t rotation) {
    transaction(SlaveCommand::motorRotation, rotation);
}

void SlaveMotor::setMotorSpeed(uint16_t speed) {
    transaction(SlaveCommand::motorSpeed, speed);
}

void SlaveSensor::init() {
    Slave::init(MASTER_CS_SENSOR);
}

uint16_t getBallAngle() {
    return transaction(SlaveCommand::ballAngle);
}

uint16_t getBallStrength() {
    return transaction(SlaveCommand::ballStrength);
}

uint16_t getLineAngle() {
    return transaction(SlaveCommand::lineAngle);
}

uint16_t getLineSize() {
    return transaction(SlaveCommand::lineSize);
}
