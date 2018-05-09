#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

uint16_t Slave::transaction(SlaveCommand command, uint16_t data) {
    dataOut[0] = (command << 10) | (data & 0x3FF);

    for (int i = 0; i < 4; i++) {
        spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);
    }

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

void SlaveMotor::setMotor(MoveData moveData) {
    setMotorAngle(moveData.angle);
    setMotorRotation(moveData.rotation);
    setMotorSpeed(moveData.speed);
}

void SlaveMotor::brake() {
    setMotor(MoveData());
}

int SlaveMotor::getLeftRPM() {
    return transaction(SlaveCommand::motorLeftRPMCommand);
}

int SlaveMotor::getRightRPM() {
    return transaction(SlaveCommand::motorRightRPMCommand);
}

int SlaveMotor::getBackLeftRPM() {
    return transaction(SlaveCommand::motorBackLeftRPMCommand);
}

int SlaveMotor::getBackRightRPM() {
    return transaction(SlaveCommand::motorBackRightRPMCommand);
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

double SlaveSensor::getLineSize() {
    return (double)transaction(SlaveCommand::lineSizeCommand) / 100.0;
}

BallData SlaveSensor::getBallData() {
    int angle = getBallAngle();
    int strength = getBallStrength();

    return BallData(angle, strength, angle != TSOP_NO_BALL);
}

int SlaveSensor::getLightSensorData() {
    uint16_t first16Bit = transaction(SlaveCommand::lsFirst16BitCommmand);
    uint16_t second16Bit = transaction(SlaveCommand::lsFirst16BitCommmand);
    return first16Bit | (second16Bit << 16);
}

void SlaveDebug::init() {
    Slave::init(MASTER_CS_DEBUG);
}

void SlaveDebug::sendLightSensorData(int data) {
    uint16_t first16Bit = data & 0xFFFF;
    uint16_t second16Bit = data >> 16;

    transaction(SlaveCommand::lsFirst16BitCommmand, first16Bit);
    transaction(SlaveCommand::lsSecond16BitCommand, second16Bit);
}

void SlaveDebug::sendPlayMode(bool isAttacker) {
    transaction(SlaveCommand::playModeCommand, isAttacker);
}

void SlaveDebug::sendBallAngle(uint16_t angle) {
    transaction(SlaveCommand::ballAngleCommand, angle);
}

void SlaveDebug::sendBallStrength(uint16_t strength) {
    transaction(SlaveCommand::ballStrengthCommand, strength);
}

DebugSettings SlaveDebug::getDebugSettings() {
    return DebugSettings(transaction(SlaveCommand::debugSettingsCommand));
}

void SlaveDebug::sendIMUIsCalibrated() {
    transaction(SlaveCommand::IMUIsCalibratedCommand);
}

void SlaveDebug::sendHeadingIsReset() {
    transaction(SlaveCommand::headingIsResetCommand);
}

void SlaveDebug::sendHeading(uint16_t heading) {
    transaction(SlaveCommand::headingCommand, heading);
}

void SlaveDebug::sendLeftRPM(uint16_t rpm) {
    transaction(SlaveCommand::motorLeftRPMCommand, rpm / 2);
}

void SlaveDebug::sendRightRPM(uint16_t rpm) {
    transaction(SlaveCommand::motorRightRPMCommand, rpm / 2);
}

void SlaveDebug::sendBackLeftRPM(uint16_t rpm) {
    transaction(SlaveCommand::motorBackLeftRPMCommand, rpm / 2);
}

void SlaveDebug::sendBackRightRPM(uint16_t rpm) {
    transaction(SlaveCommand::motorBackRightRPMCommand, rpm / 2);
}
