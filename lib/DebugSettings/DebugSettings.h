#ifndef DEBUG_SETTINGS_H
#define DEBUG_SETTINGS_H

typedef struct DebugSettings {
    bool engineStarted;
    bool IMUNeedsResetting;
    bool playModeSwitching;
    bool lightSensorsNeedResetting;
    bool defaultPlayModeIsAttack;

    DebugSettings() {
        engineStarted = false;
        IMUNeedsResetting = false;
        playModeSwitching = false;
        lightSensorsNeedResetting = false;
        defaultPlayModeIsAttack = true;
    }

    DebugSettings(uint16_t numberValue) {
        engineStarted = numberValue & 0x1;
        IMUNeedsResetting = (numberValue >> 1) & 0x1;
        playModeSwitching = (numberValue >> 2) & 0x1;
        lightSensorsNeedResetting = (numberValue >> 3) & 0x1;
        defaultPlayModeIsAttack = (numberValue >> 4) & 0x1;
    }

    uint16_t numberValue() {
        return engineStarted | (IMUNeedsResetting << 1) | (playModeSwitching << 2) | (lightSensorsNeedResetting << 3) | (defaultPlayModeIsAttack << 4);
    }
} DebugSettings;

#endif // DEBUG_SETTINGS_H
