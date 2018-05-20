#ifndef BLUETOOTHDATATYPE_H
#define BLUETOOTHDATATYPE_H

#include <Arduino.h>

enum BluetoothDataType: int {
    noData,
    settings,
    info,
    ball,
    ballStr,
    heading,
    lineAngle,
    lineSize//,
    // lightSensor,
    // compass,
    // raw,
    // btLinePosition,
    // btRobotPosition,
    // orbitAngle,
    // goal
};

// typedef struct BluetoothDataType {
//     BluetoothDataType type;
//     int value;
//     String string;

//     BluetoothData() {}
//     BluetoothData(BluetoothDataType t, int v, String s) : type(t), value(v), string(s) {}
// } BluetoothData;

#endif
