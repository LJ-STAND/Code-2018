#ifndef DEBUG_SETTINGS_H
#define DEBUG_SETTINGS_H

typedef struct DebugSettings {
    bool engineStarted;
    bool headingNeedsResetting;
    bool IMUNeedsCalibrating;

    // bool playModeSwitching;

    DebugSettings() {
        engineStarted = false;
        headingNeedsResetting = false;
        IMUNeedsCalibrating = false;
    }

    DebugSettings(uint16_t numberValue) {
        engineStarted = numberValue & 0x1;
        headingNeedsResetting = (numberValue >> 1) & 0x1;
        IMUNeedsCalibrating = (numberValue >> 2) & 0x1;
    }

    uint16_t numberValue() {
        return engineStarted | (headingNeedsResetting << 1) | (IMUNeedsCalibrating << 2);
    }
} DebugSettings;

#endif // DEBUG_SETTINGS_H
