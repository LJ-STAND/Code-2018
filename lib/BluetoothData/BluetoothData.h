#ifndef BLUETOOTH_DATA_H
#define BLUETOOTH_DATA_H

#include <Arduino.h>
#include <BallData.h>
#include <Point.h>
#include <PlayMode.h>

typedef struct BluetoothData {
    BallData ballData;
    uint16_t heading;
    bool ballIsOut;
    PlayMode playMode;
    bool isOnField;
    Point robotPosition;

    BluetoothData() {
        ballData = BallData();
        heading = 0;
        ballIsOut = false;
        playMode = PlayMode::attackMode;
        isOnField = true;
        robotPosition = Point();
    }

    BluetoothData(BallData ballData, uint16_t heading, bool ballIsOut, PlayMode playMode, bool isOnField, Point robotPosition) : ballData(ballData), heading(heading), ballIsOut(ballIsOut), playMode(playMode), isOnField(isOnField), robotPosition(robotPosition) {}
} BluetoothData;

#endif // BLUETOOTH_DATA_H