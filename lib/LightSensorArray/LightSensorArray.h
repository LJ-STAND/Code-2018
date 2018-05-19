#ifndef LIGHT_SENSOR_ARRAY_H
#define LIGHT_SENSOR_ARRAY_H

#include <Arduino.h>
#include <Pins.h>
#include <Common.h>
#include <Config.h>
#include <EEPROM.h>

class LightSensorArray {
public:
    LightSensorArray() {}

    void init();

    void read();
    int readSensor(int sensor);
    void changeMUXChannel(uint8_t channel);

    void calculateClusters(bool doneFillInSensors = false);
    void fillInSensors();
    void calculateLine();
    void resetStartEnds();

    void calibrate();

    double getLineAngle();
    double getLineSize();

    bool data[LS_NUM];
    bool filledInData[LS_NUM];

    uint16_t thresholds[LS_NUM];

    int starts[4];
    int ends[4];

    int numClusters = 0;

private:
    void resetClusters();

    uint8_t muxChannels[32] = {12, 11, 10, 9, 13, 14, 15, 16, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 1, 2, 3, 4, 5, 6, 7, 8};

    double angle;
    double size;
};

#endif // LIGHT_SENSOR_ARRAY_H
