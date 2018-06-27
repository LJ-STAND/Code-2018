#ifndef TSOP_ARRAY_H
#define TSOP_ARRAY_H

#include <Arduino.h>

#include <Common.h>
#include <Config.h>
#include <Pins.h>

// IR Sensor array object
class TSOPArray {
public:
    TSOPArray() {}
    void init();
    void updateOnce();
    void finishRead();
    void sortValues();
    void calculateAngleStrength(uint8_t n);

    uint16_t getAngle();
    uint16_t getStrength();

    uint16_t values[TSOP_NUM] = {0}; // Sensor values as percentages
    uint16_t sortedValues[TSOP_NUM] = {0}; // Sorted values (highest to lowest)
    uint8_t indexes[TSOP_NUM] = {0}; // Indexes of sensor numbers in sorted list
    uint16_t tsopCounter = 0; // Number of reads

private:
    uint16_t tempValues[TSOP_NUM] = {0}; // Raw sensor values after read

    uint16_t angle = 0;
    uint16_t strength = 0;

    uint8_t TSOPPins[TSOP_NUM] = {TSOP_0, TSOP_1, TSOP_2, TSOP_3, TSOP_4, TSOP_5, TSOP_6, TSOP_7, TSOP_8, TSOP_9, TSOP_10, TSOP_11, TSOP_12, TSOP_13, TSOP_14, TSOP_15, TSOP_16, TSOP_17, TSOP_18, TSOP_19, TSOP_20, TSOP_21, TSOP_22, TSOP_23};

    // Scaled sin and cos tables
    double scaledSin[TSOP_NUM] = {0};
    double scaledCos[TSOP_NUM] = {0};
};

#endif // TSOP_ARRAY_H
