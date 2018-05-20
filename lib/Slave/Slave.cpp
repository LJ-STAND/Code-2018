#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

void Slave::transaction(SlaveCommand command, uint16_t data, uint8_t numSend) {
    dataOut[0] = (command << 10) | (data & 0x3FF);

    for (int i = 0; i < numSend; i++) {
        spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);
    }

    handleReceive((dataIn[0] >> 10), dataIn[0] & 0x3FF);
}

void SlaveMotor::init() {
    Slave::init(MASTER_CS_MOTOR);
}

void SlaveMotor::handleReceive(uint8_t command, uint16_t data) {
    switch (command) {
    case SlaveCommand::motorLeftRPMCommand:
        leftRPM = data * 2;
        break;

    case SlaveCommand::motorRightRPMCommand:
        rightRPM = data * 2;
        break;

    case SlaveCommand::motorBackLeftRPMCommand:
        backLeftRPM = data * 2;
        break;

    case SlaveCommand::motorBackRightRPMCommand:
        backRightRPM = data * 2;
        break;
    }
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

void SlaveMotor::updateLeftRPM() {
    transaction(SlaveCommand::motorLeftRPMCommand);
}

void SlaveMotor::updateRightRPM() {
    transaction(SlaveCommand::motorRightRPMCommand);
}

void SlaveMotor::updateBackLeftRPM() {
    transaction(SlaveCommand::motorBackLeftRPMCommand);
}

void SlaveMotor::updateBackRightRPM() {
    transaction(SlaveCommand::motorBackRightRPMCommand);
}

void SlaveSensor::init() {
    Slave::init(MASTER_CS_SENSOR);
}

void SlaveSensor::handleReceive(uint8_t command, uint16_t data) {
    switch (command) {
    case SlaveCommand::ballAngleCommand:
        ballAngle = data;
        break;

    case SlaveCommand::ballStrengthCommand:
        ballStrength = data;
        break;

    case SlaveCommand::lineAngleCommand:
        lineAngle = data;
        break;

    case SlaveCommand::lineSizeCommand:
        lineSize = (double)data / 100.0;
        break;
    }
}

void SlaveSensor::updateBallAngle() {
    transaction(SlaveCommand::ballAngleCommand);
}

void SlaveSensor::updateBallStrength() {
    transaction(SlaveCommand::ballStrengthCommand);
}

void SlaveSensor::updateLineAngle() {
    transaction(SlaveCommand::lineAngleCommand);
}

void SlaveSensor::updateLineSize() {
    transaction(SlaveCommand::lineSizeCommand);
}

void SlaveSensor::updateBallData() {
    updateBallAngle();
    updateBallStrength();
}

BallData SlaveSensor::ballData() {
    return BallData(ballAngle, ballStrength);
}

void SlaveDebug::sendLightSensorData(int data) {
    uint8_t firstByte = data & 0xFF;
    uint8_t secondByte = (data >> 8) & 0xFF;
    uint8_t thirdByte = (data >> 16) & 0xFF;
    uint8_t fourthByte = (data >> 24) & 0xFF;

    transaction(SlaveCommand::lsFirstByteCommand, firstByte);
    transaction(SlaveCommand::lsSecondByteCommand, secondByte);
    transaction(SlaveCommand::lsThirdByteCommand, thirdByte);
    transaction(SlaveCommand::lsFourthByteCommand, fourthByte);
}

void SlaveSensor::updateLightSensorData() {}

void SlaveSensor::sendCalibrateLightSensors() {
    transaction(SlaveCommand::calibrateLightSensorsCommand, 0, 1);
}

void SlaveDebug::init() {
    Slave::init(MASTER_CS_DEBUG);
}

void SlaveDebug::handleReceive(uint8_t command, uint16_t data) {
    switch (command) {
        case SlaveCommand::debugSettingsCommand:
        debugSettings = DebugSettings(data);
    }
}

void SlaveDebug::sendPlayMode(bool isAttacker) {
    transaction(SlaveCommand::playModeCommand, isAttacker);
}

void SlaveDebug::sendBallData(BallData ballData) {
    transaction(SlaveCommand::ballAngleCommand, ballData.angle);
    transaction(SlaveCommand::ballStrengthCommand, ballData.strength);
}

void SlaveDebug::updateDebugSettings() {
    transaction(SlaveCommand::debugSettingsCommand);
}

void SlaveDebug::sendIMUIsReset() {
    transaction(SlaveCommand::IMUIsResetCommand, 0, 1);
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


void SlaveDebug::sendLineData(uint16_t angle, uint16_t size) {
    transaction(SlaveCommand::lineAngleCommand, angle);
    delay(10);
    transaction(SlaveCommand::lineSizeCommand, size);
}

void SlaveDebug::sendLightSensorsAreReset() {
    transaction(SlaveCommand::lightSensorsAreResetCommand, 0, 1);
}

void SlaveDebug::sendLineData(LineData data) {
    transaction(SlaveCommand::lineAngleCommand, data.angle);
    transaction(SlaveCommand::lineSizeCommand, data.size * 100);
    transaction(SlaveCommand::lineOnFieldCommand, data.onField);
}

size_t SlaveDebug::write(uint8_t c) {
    transaction(SlaveCommand::debugTerminalCommand, c, 1);
}