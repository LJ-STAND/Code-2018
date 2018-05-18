#ifndef DEBUG_SETTINGS_H
#define DEBUG_SETTINGS_H

#include <Arduino.h>
#include <Config.h>
#include <EEPROM.h>

class DebugSettings {
public:
    DebugSettings();
    DebugSettings(uint16_t numberValue);

    uint16_t numberValue();
    void setNumberValue(uint16_t numberValue);

    void readEEPROM();
    void writeEEPROM();

    bool engineStarted;
    bool IMUNeedsResetting;
    bool playModeSwitching;
    bool lightSensorsNeedResetting;
    bool defaultPlayModeIsAttack;
    bool goalIsYellow;
};

#endif // DEBUG_SETTINGS_H
