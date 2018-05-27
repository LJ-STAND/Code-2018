/* Library for interfacing with TSOP array
 */
#include "TSOPArray.h"

void TSOPArray::init() {
    // Set the correct pinmodes for all the TSOP pins
    pinMode(TSOP_PWR_1, OUTPUT);
    pinMode(TSOP_PWR_2, OUTPUT);
    pinMode(TSOP_PWR_3, OUTPUT);
    pinMode(TSOP_PWR_4, OUTPUT);
    pinMode(TSOP_PWR_5, OUTPUT);
    pinMode(TSOP_PWR_6, OUTPUT);
    pinMode(TSOP_PWR_7, OUTPUT);

    digitalWrite(TSOP_PWR_1, HIGH);
    digitalWrite(TSOP_PWR_2, HIGH);
    digitalWrite(TSOP_PWR_3, HIGH);
    digitalWrite(TSOP_PWR_4, HIGH);
    digitalWrite(TSOP_PWR_5, HIGH);
    digitalWrite(TSOP_PWR_6, HIGH);
    digitalWrite(TSOP_PWR_7, HIGH);

    for (uint8_t i = 0; i < TSOP_NUM; i++) {
        pinMode(TSOPPins[i], INPUT);
    }

    // Set up scaled sin/cos tables

    double temp_angle;

    for (int i = 0; i < TSOP_NUM; i++){
        temp_angle = degreesToRadians(i * TSOP_NUM_MULTIPLIER);

        scaledCos[i] = cos(temp_angle);
        scaledSin[i] = sin(temp_angle);
    }
}

void TSOPArray::updateOnce() {
    // Read each TSOP once
    tempValues[0] += digitalReadFast(TSOP_0) ^ 1;
    tempValues[1] += digitalReadFast(TSOP_1) ^ 1;
    tempValues[2] += digitalReadFast(TSOP_2) ^ 1;
    tempValues[3] += digitalReadFast(TSOP_3) ^ 1;
    tempValues[4] += digitalReadFast(TSOP_4) ^ 1;
    tempValues[5] += digitalReadFast(TSOP_5) ^ 1;
    tempValues[6] += digitalReadFast(TSOP_6) ^ 1;
    tempValues[7] += digitalReadFast(TSOP_7) ^ 1;
    tempValues[8] += digitalReadFast(TSOP_8) ^ 1;
    tempValues[9] += digitalReadFast(TSOP_9) ^ 1;
    tempValues[10] += digitalReadFast(TSOP_10) ^ 1;
    tempValues[11] += digitalReadFast(TSOP_11) ^ 1;
    tempValues[12] += digitalReadFast(TSOP_12) ^ 1;
    tempValues[13] += digitalReadFast(TSOP_13) ^ 1;
    tempValues[14] += digitalReadFast(TSOP_14) ^ 1;
    tempValues[15] += digitalReadFast(TSOP_15) ^ 1;
    tempValues[16] += digitalReadFast(TSOP_16) ^ 1;
    tempValues[17] += digitalReadFast(TSOP_17) ^ 1;
    tempValues[18] += digitalReadFast(TSOP_18) ^ 1;
    tempValues[19] += digitalReadFast(TSOP_19) ^ 1;
    tempValues[20] += digitalReadFast(TSOP_20) ^ 1;
    tempValues[21] += digitalReadFast(TSOP_21) ^ 1;
    tempValues[22] += digitalReadFast(TSOP_22) ^ 1;
    tempValues[23] += digitalReadFast(TSOP_23) ^ 1;

    tsopCounter++;
}

void TSOPArray::finishRead() {
    // Complete a reading of the TSOPs after a certain amount of individual readings, TSOP values are now stored in the values array until the next complete read
    for (uint8_t i = 0; i < TSOP_NUM; i++) {
        #if DEBUG_TSOP
            Serial.print(tempValues[i]);
            if (i != TSOP_NUM - 1) {
                Serial.print(", ");
            } else {
                Serial.println();
            }
        #endif

        values[i] = 100 * (double)tempValues[i] / (double)tsopCounter;
        tempValues[i] = 0;
        sortedValues[i] = 0;
        indexes[i] = 0;
    }

    tsopCounter = 0;

    sortValues();
    calculateAngleStrength(TSOP_BEST_TSOP_NUMBER);
}

void TSOPArray::sortValues() {
    /* Sort the TSOP values from greatest to least in sortedFilteredValues
     * and sort the TSOP indexes from greatest to least strength in indexes
     */
    for (uint8_t i = 0; i < TSOP_NUM; i++) {
        for (uint8_t j = 0; j < TSOP_NUM; j++) {
            if (values[i] > sortedValues[j]) {
                // We've found our place!
                // Shift elements from index j down
                if (j <= i) {
                    // Make sure we only shift what is needed
                    ARRAYSHIFTDOWN(sortedValues, j, i);
                    ARRAYSHIFTDOWN(indexes, j, i);
                }

                sortedValues[j] = values[i];
                indexes[j] = i;
                break;
            }
        }
    }
}

void TSOPArray::calculateAngleStrength(uint8_t n) {
    // Cartesian addition of best n TSOPs

    int16_t x = 0;
    int16_t y = 0;

    for (uint8_t i = 0; i < n; i++) {
        // convert vector to cartesian
        x += sortedValues[i] * scaledCos[indexes[i]];
        y += sortedValues[i] * scaledSin[indexes[i]];
    }

    if (x == 0 && y == 0) {
        // When vectors sum to (0, 0), we're in trouble. We've got some dodgy data
        angle = TSOP_NO_BALL;
    } else {
        angle = mod(radiansToDegrees(atan2(y, x)), 360);
    }

    strength = sqrt(x * x + y * y);
}

uint16_t TSOPArray::getAngle() {
    return angle;
}

uint16_t TSOPArray::getStrength() {
    return strength;
}