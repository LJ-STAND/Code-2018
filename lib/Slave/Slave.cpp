#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

uint16_t Slave::transaction(SlaveCommand command, uint16_t data) {
    dataOut[0] = (command << 10) | (data & 0x3FF);

    spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);
    spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);

    return dataIn[0];
}

void SlaveMotor::init() {
    Slave::init(MASTER_CS_MOTOR);
}

void SlaveMotor::setMotorAngle(uint16_t angle) {
    transaction(SlaveCommand::motorAngleCommand, angle);
}

void SlaveMotor::setMotorRotation(uint16_t rotation) {
    transaction(SlaveCommand::motorRotationCommand, rotation);
}

void SlaveMotor::setMotorSpeed(uint16_t speed) {
    transaction(SlaveCommand::motorSpeedCommand, speed);
}

void SlaveSensor::init() {
    Slave::init(MASTER_CS_SENSOR);
}

uint16_t SlaveSensor::getBallAngle() {
    return transaction(SlaveCommand::ballAngleCommand);
}

uint16_t SlaveSensor::getBallStrength() {
    return transaction(SlaveCommand::ballStrengthCommand);
}

uint16_t SlaveSensor::getLineAngle() {
    return transaction(SlaveCommand::lineAngleCommand);
}

uint16_t SlaveSensor::getLineSize() {
    return transaction(SlaveCommand::lineSizeCommand);
}
