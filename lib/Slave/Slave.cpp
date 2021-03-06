#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

void Slave::transaction(SlaveCommand command, uint16_t data, uint8_t numSend) {
    // Combine the data and command into one 16 bit number. First 10 bits are data, last 6 are command
    dataOut[0] = (command << 10) | (data & 0x3FF);

    // Send the data numSend times
    for (int i = 0; i < numSend; i++) {
        spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);
    }

    // Recieve
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

    case SlaveCommand::lsFirstCommand:
        lsFirst = data;
        break;

    case SlaveCommand::lsSecondCommand:
        lsSecond = data;
        break;

    case SlaveCommand::lsThirdCommand:
        lsThird = data;
        break;

    case SlaveCommand::lsFourthCommand:
        lsFourth = data;
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

void SlaveSensor::updateLightSensorData() {
    transaction(SlaveCommand::lsFirstCommand);
    transaction(SlaveCommand::lsSecondCommand);
    transaction(SlaveCommand::lsThirdCommand);
    transaction(SlaveCommand::lsFourthCommand);
}

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

void SlaveDebug::sendLightSensorData(uint16_t first, uint16_t second, uint16_t third, uint16_t fourth) {
    transaction(SlaveCommand::lsFirstCommand, first);
    transaction(SlaveCommand::lsSecondCommand, second);
    transaction(SlaveCommand::lsThirdCommand, third);
    transaction(SlaveCommand::lsFourthCommand, fourth);
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

void SlaveDebug::sendLightSensorsAreReset() {
    transaction(SlaveCommand::lightSensorsAreResetCommand, 0, 1);
}

void SlaveDebug::sendLineData(LineData data) {
    transaction(SlaveCommand::lineAngleCommand, data.angle);
    transaction(SlaveCommand::lineSizeCommand, data.size * 100);
    transaction(SlaveCommand::lineOnFieldCommand, data.onField);
}

void SlaveDebug::sendGoals(uint16_t yellowAngle, uint16_t yellowDistance, uint16_t blueAngle, uint16_t blueDistance) {
    transaction(SlaveCommand::yellowAngleCommand, yellowAngle);
    transaction(SlaveCommand::yellowDistanceCommand, yellowDistance);
    transaction(SlaveCommand::blueAngleCommand, blueAngle);
    transaction(SlaveCommand::blueDistanceCommand, blueDistance);
}

void SlaveDebug::sendRobotPosition(Point robotPosition) {
    transaction(SlaveCommand::robotPositionXCommand, (int8_t)robotPosition.x);
    transaction(SlaveCommand::robotPositionYCommand, (int8_t)robotPosition.y);
}

size_t SlaveDebug::write(uint8_t c) {
    transaction(SlaveCommand::debugTerminalCommand, c, 1);
    return 1;
}

void SlaveDebug::sendBluetoothConnected(bool bluetoothConnected) {
    transaction(SlaveCommand::bluetoothConnectedCommand, bluetoothConnected);
}

void SlaveDebug::sendBluetoothData(BluetoothData bluetoothData) {
    transaction(SlaveCommand::bluetoothDataBallAngleCommand, bluetoothData.ballData.angle);
    transaction(SlaveCommand::bluetoothDataBallStrengthCommand, bluetoothData.ballData.strength);
    transaction(SlaveCommand::bluetoothDataHeadingCommand, bluetoothData.heading);
    transaction(SlaveCommand::bluetoothDataBallIsOutCommand, bluetoothData.ballIsOut);
    transaction(SlaveCommand::bluetoothDataIsOnFieldCommand, bluetoothData.isOnField);
    transaction(SlaveCommand::bluetoothDataRobotPositionXCommand, (int8_t)bluetoothData.robotPosition.x);
    transaction(SlaveCommand::bluetoothDataRobotPositionYCommand, (int8_t)bluetoothData.robotPosition.y);
}