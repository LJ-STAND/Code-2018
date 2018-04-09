#ifndef LIGHT_SENSOR_ARRAY_H
#define LIGHT_SENSOR_ARRAY_H

#include <Arduino.h>
#include <Pins.h>
#include <LightSensor.h>
#include <LightSensorCluster.h>
#include <Bits.h>
#include <Common.h>

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

    double getLineAngle();
    double getLineSize();

    uint16_t getFirst16Bit();
    uint16_t getSecond16Bit();

    LightSensor sensors[LS_NUM];
    bool data[LS_NUM];
    bool filledInData[LS_NUM];

    int thresholds[LS_NUM];

    LightSensorCluster cluster1 = LightSensorCluster(0.0, 0);
    LightSensorCluster cluster2 = LightSensorCluster(0.0, 0);
    LightSensorCluster cluster3 = LightSensorCluster(0.0, 0);

    int numClusters = 0;

private:
    void resetClusters();

    uint8_t muxChannels[32] = {12, 11, 10, 9, 13, 14, 15, 16, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 1, 2, 3, 4, 5, 6, 7, 8};

    double angle;
    double size;
};

#endif // LIGHT_SENSOR_ARRAY_H
