/* Library for interfacing with TSOP array
 */

#ifndef TSOP_ARRAY_H
#define TSOP_ARRAY_H

#include <Arduino.h>

#include <Common.h>
#include <Config.h>
#include <Pins.h>

class TSOPArray {
public:
    TSOPArray() {}
    void init();
    void updateOnce();
    void on();
    void off();
    void unlock();
    void finishRead();
    void sortFilterValues();
    void calculateAngleSimple();
    void calculateAngle(uint8_t n);
    void calculateStrengthSimple();
    void calculateStrength(uint8_t n);
    uint16_t getAngle();
    uint16_t getStrength();
    uint16_t getSimpleStrength();

    uint16_t values[TSOP_NUM] = {0};
    uint16_t filteredValues[TSOP_NUM] = {0};
    uint16_t sortedFilteredValues[TSOP_NUM] = {0};
    uint8_t indexes[TSOP_NUM] = {0};
    uint16_t tsopCounter = 0;

private:
    uint16_t angle = 0;
    uint16_t simpleAngle = 0;
    uint16_t strength = 0;
    uint16_t simpleStrength = 0;

    uint16_t tempValues[TSOP_NUM] = {0};
    uint16_t tempFilteredValues[TSOP_NUM] = {0};
    uint8_t TSOPPins[TSOP_NUM] = {TSOP_0, TSOP_1, TSOP_2, TSOP_3, TSOP_4, TSOP_5, TSOP_6, TSOP_7, TSOP_8, TSOP_9, TSOP_10, TSOP_11, TSOP_12, TSOP_13, TSOP_14, TSOP_15, TSOP_16, TSOP_17, TSOP_18, TSOP_19, TSOP_20, TSOP_21, TSOP_22, TSOP_23};
    double scaledSin[TSOP_NUM] = {0};
    double scaledCos[TSOP_NUM] = {0};
};

#endif // TSOP_ARRAY_H
