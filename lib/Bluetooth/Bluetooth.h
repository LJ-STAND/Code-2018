#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <Config.h>
#include <PlayMode.h>
#include <Timer.h>

typedef struct BluetoothData {
    uint16_t ballAngle;
    uint8_t ballStrength;
    uint16_t heading;
    bool ballIsOut;
    PlayMode playMode;
    bool isOnField;

    BluetoothData() {
        ballAngle = TSOP_NO_BALL;
        ballStrength = 0;
        heading = 0;
        ballIsOut = false;
        playMode = PlayMode::undecidedMode;
        isOnField = true;
    }

    BluetoothData(uint16_t ballAngle, uint8_t ballStrength, uint16_t heading, bool ballIsOut, PlayMode playMode, bool isOnField) : ballAngle(ballAngle), ballStrength(ballStrength), heading(heading), ballIsOut(ballIsOut), playMode(playMode), isOnField(isOnField) {}
} BluetoothData;

class Bluetooth {
public:
    void init();
    void update(BluetoothData data);

    BluetoothData otherData = BluetoothData();
    bool isConnected = false;
    bool previouslyConnected = false;

private:
    BluetoothData thisData = BluetoothData();

    Timer connectedTimer = Timer(BLUETOOTH_LOST_COMMUNICATION_TIME);
    
    void send();
    void recieve();
};

#endif // BLUETOOTH_H