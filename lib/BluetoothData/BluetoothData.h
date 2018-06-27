#ifndef BLUETOOTH_DATA_H
#define BLUETOOTH_DATA_H

#include <Arduino.h>
#include <BallData.h>
#include <Point.h>
#include <PlayMode.h>

// Struct to hold all the variables sent over Bluetooth
typedef struct BluetoothData {
    BallData ballData; // Ball info
    uint16_t heading; // Robot heading
    bool ballIsOut; // Ball is out
    PlayMode playMode; // Play mode
    bool isOnField; // Robot is on the field
    Point robotPosition; // Position of the robot

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