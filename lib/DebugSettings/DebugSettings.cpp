#include "DebugSettings.h"

DebugSettings::DebugSettings() {
    engineStarted = false;
    IMUNeedsResetting = false;
    playModeSwitching = true;
    lightSensorsNeedResetting = false;
    defaultPlayModeIsAttack = true;
    goalIsYellow = true;
}

DebugSettings::DebugSettings(uint16_t numberValue) {
    setNumberValue(numberValue);
}

void DebugSettings::setNumberValue(uint16_t numberValue) {
    engineStarted = numberValue & 0x1;
    IMUNeedsResetting = (numberValue >> 1) & 0x1;
    playModeSwitching = (numberValue >> 2) & 0x1;
    lightSensorsNeedResetting = (numberValue >> 3) & 0x1;
    defaultPlayModeIsAttack = (numberValue >> 4) & 0x1;
    goalIsYellow = (numberValue >> 5) & 0x1;
}

uint16_t DebugSettings::numberValue() {
    return engineStarted | (IMUNeedsResetting << 1) | (playModeSwitching << 2) | (lightSensorsNeedResetting << 3) | (defaultPlayModeIsAttack << 4) | (goalIsYellow << 5);
}

void DebugSettings::writeEEPROM() {
    uint8_t number = playModeSwitching | (defaultPlayModeIsAttack << 1) | (goalIsYellow << 2);
    EEPROM.write(DEBUG_SETTINGS_EEPROM_ADDRESS, number);
}

void DebugSettings::readEEPROM() {
    uint8_t number = EEPROM.read(DEBUG_SETTINGS_EEPROM_ADDRESS);
    playModeSwitching = number & 0x1;
    defaultPlayModeIsAttack = (number >> 1) & 0x1;
    goalIsYellow = (number >> 2) & 0x1;
}

