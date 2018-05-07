#ifndef DEBUG_CONTROLLER_H
#define DEBUG_CONTROLLER_H

#include <Pins.h>
#include <Arduino.h>
#include <Bluetooth.h>
#include <Bits.h>
#include <Config.h>
#include <Timer.h>
#include <BluetoothData.h>

class DebugController {
public:
    DebugController() {}
    void init();
    void appSendTSOPs(int tsop, int strength);
    void appSendIMU(double angle);
    void appSendTSOPs(double angle);
    void appSendOrbitAngle(double angle);
    void appSendString(String string);
    void appSendLightSensors(uint16_t first16Bit, uint16_t second16Bit);
    void appSendPixy(double x, double y, double width, double height);
    void appSendRobotPosition(double lineAngle, double lineSize);
};

#endif
