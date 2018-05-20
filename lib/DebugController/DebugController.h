#ifndef DEBUG_CONTROLLER_H
#define DEBUG_CONTROLLER_H

#include <Pins.h>
#include <Arduino.h>
#include <Bluetooth.h>
#include <Config.h>
#include <Timer.h>

enum AppDataType: int {
    noData,
    settings,
    info,
    ball,
    ballStr,
    heading,
    lineAngle,
    lineSize
};

class DebugController {
public:
    DebugController() {}
    void init();
    void send(String data, AppDataType dataCode = AppDataType::info);
    void appSendBallAngle(int angle);
    void appSendBallStrength(int strength);
    void appSendString(String string);
    void appSendHeading(int heading);
    void appSendLineAngle(int angle);
    void appSendLineSize(int size);
    // void appSendCompass(double angle);
    // void appSendTSOPs(double angle);
    // void appSendOrbitAngle(double angle);
    // void appSendString(String string);
    // void appSendLightSensors(uint16_t first16Bit, uint16_t second16Bit);
    // void appSendRobotPosition(double lineAngle, double lineSize);
};

#endif
