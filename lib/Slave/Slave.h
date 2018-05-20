#ifndef SLAVE_H
#define SLAVE_H

#include <t3spi.h>
#include <Pins.h>
#include <Config.h>
#include <BallData.h>
#include <MoveData.h>
#include <DebugSettings.h>
#include <LineData.h>

enum SlaveCommand: uint8_t {
    motorAngleCommand,
    motorRotationCommand,
    motorSpeedCommand,
    ballAngleCommand,
    ballStrengthCommand,
    lineAngleCommand,
    lineSizeCommand,
    lineOnFieldCommand,
    lsFirst16BitCommmand,
    lsSecond16BitCommand,
    playModeCommand,
    debugSettingsCommand,
    IMUIsResetCommand,
    headingCommand,
    motorLeftRPMCommand,
    motorRightRPMCommand,
    motorBackLeftRPMCommand,
    motorBackRightRPMCommand,
    lightSensorsAreResetCommand,
    calibrateLightSensorsCommand,
    debugTerminalCommand,
    yellowAngleCommand,
    yellowDistanceCommand,
    blueAngleCommand,
    blueDistanceCommand
};

class Slave {
public:
    void init(int csPin);
    void transaction(SlaveCommand command, uint16_t data = 0, uint8_t numSend = 2);

private:
    virtual void handleReceive(uint8_t command, uint16_t data);

    volatile uint16_t dataIn[1];
    volatile uint16_t dataOut[1];
    int cs;
};

class SlaveMotor: public Slave {
public:
    void init();
    void handleReceive(uint8_t command, uint16_t data);
    void setMotorAngle(uint16_t angle);
    void setMotorRotation(uint16_t rotation);
    void setMotorSpeed(uint16_t speed);
    void setMotor(MoveData moveData);
    void brake();
    void updateLeftRPM();
    void updateRightRPM();
    void updateBackLeftRPM();
    void updateBackRightRPM();

    int leftRPM;
    int rightRPM;
    int backLeftRPM;
    int backRightRPM;
};

class SlaveSensor: public Slave {
public:
    void init();
    void handleReceive(uint8_t command, uint16_t data);
    void updateBallAngle();
    void updateBallStrength();
    void updateBallData();

    void updateLineAngle();
    void updateLineSize();
    void updateLightSensorData();
    void sendCalibrateLightSensors();

    uint16_t ballAngle;
    uint16_t ballStrength;
    BallData ballData();

    uint16_t lineAngle;
    double lineSize;
    int lightSensorData;
};

class SlaveDebug: public Slave, public Print {
public:
    void init();
    void handleReceive(uint8_t command, uint16_t data);
    void sendLightSensorData(int data);
    void sendLineData(LineData data);
    void sendPlayMode(bool isAttacker);
    void sendBallData(BallData ballData);
    void sendIMUIsReset();
    void sendLightSensorsAreReset();
    void sendHeading(uint16_t heading);
    void sendLeftRPM(uint16_t rpm);
    void sendRightRPM(uint16_t rpm);
    void sendBackLeftRPM(uint16_t rpm);
    void sendBackRightRPM(uint16_t rpm);
    void sendGoals(uint16_t yellowAngle, uint16_t yellowDistance, uint16_t blueAngle, uint16_t blueDistance);

    void updateDebugSettings();

    size_t write(uint8_t c);

    DebugSettings debugSettings;
};

#endif // SLAVE_H
