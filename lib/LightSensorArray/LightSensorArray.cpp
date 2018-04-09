#include "LightSensorArray.h"

void LightSensorArray::init() {
    pinMode(MUX_EN, OUTPUT);
    pinMode(MUX_WR, OUTPUT);
    pinMode(MUX_CS, OUTPUT);
    pinMode(MUX_A0, OUTPUT);
    pinMode(MUX_A1, OUTPUT);
    pinMode(MUX_A2, OUTPUT);
    pinMode(MUX_A3, OUTPUT);
    pinMode(MUX_A4, OUTPUT);

    pinMode(LS0, INPUT);
    pinMode(LS1, INPUT);
    pinMode(LS2, INPUT);
    pinMode(LS3, INPUT);
    pinMode(LS_MUX, INPUT);

    digitalWrite(MUX_CS, LOW);
    digitalWrite(MUX_EN, LOW);
}

void LightSensorArray::changeMUXChannel(uint8_t channel) {
    digitalWriteFast(MUX_WR, LOW);

    digitalWriteFast(MUX_A0, channel & 0x1);
    digitalWriteFast(MUX_A1, (channel >> 1) & 0x1);
    digitalWriteFast(MUX_A2, (channel >> 2) & 0x1);
    digitalWriteFast(MUX_A3, (channel >> 3) & 0x1);
    digitalWriteFast(MUX_A4, (channel >> 4) & 0x1);

    digitalWriteFast(MUX_WR, HIGH);
}

int LightSensorArray::readSensor(int sensor) {
    if (sensor == 0) {
        return analogRead(LS0);
    } else if (sensor == 1) {
        return analogRead(LS1);
    } else if (sensor == 2) {
        return analogRead(LS2);
    } else if (sensor == 3) {
        return analogRead(LS3);
    } else {
        changeMUXChannel(muxChannels[sensor - 4] - 1);
        return analogRead(LS_MUX);
    }
}

void LightSensorArray::read() {
    for (int i = 0; i < LS_NUM; i++) {
        data[i] = readSensor(i) > thresholds[i];
    }
}

void LightSensorArray::calculateClusters(bool doneFillInSensors) {
    bool *lightData = !doneFillInSensors ? data : filledInData;

    resetStartEnds();

    int index = 0;
    bool previousValue = false;

    for (int i = 0; i < LS_NUM; i++) {
        if (lightData[i] && !previousValue) {
            starts[index] = i;
        }

        if (!lightData[i] && previousValue) {
            ends[index] = i - 1;
            index++;

            if (index > 3) {
                // Too many clusters.
                if (!doneFillInSensors) {
                    fillInSensors();
                } else {
                    resetStartEnds();
                    numClusters = 0;
                }
                return;
            }
        }
    }

    int tempNumClusters = (int)(starts[0] != LS_ES_DEFAULT) + (int)(starts[1] != LS_ES_DEFAULT) + (int)(starts[2] != LS_ES_DEFAULT) + (int)(starts[3] != LS_ES_DEFAULT);
    if (tempNumClusters != index) {
        // If the final cluster didn't end, index will be one less than tempNumClusters.
        starts[0] = starts[index];
    }
    numClusters = index;
}

void LightSensorArray::fillInSensors() {
    for (int i = 0; i < LS_NUM; i++) {
        filledInData[i] = data[i];

        if (!data[i] && data[mod(i - 1, LS_NUM)] && data[mod(i + 1, LS_NUM)]) {
            filledInData[i] = true;
        }
    }

    calculateClusters(true);
}

void LightSensorArray::calculateLine() {
    if (numClusters == 0) {
        angle = NO_LINE_ANGLE;
        size = NO_LINE_SIZE;
    } else {
        double cluster1Angle = midAngleBetween(start[0] * LS_NUM_MULTIPLIER, end[0] * LS_NUM_MULTIPLIER);
        double cluster2Angle = midAngleBetween(start[1] * LS_NUM_MULTIPLIER, end[1] * LS_NUM_MULTIPLIER);
        double cluster3Angle = midAngleBetween(start[2] * LS_NUM_MULTIPLIER, end[2] * LS_NUM_MULTIPLIER);

        if (numClusters == 1) {
            angle = cluster1Angle;
            size = 1 - cos(degreesToRadians(angleBetween(starts[0] * LS_NUM_MULTIPLIER, ends[0] * LS_NUM_MULTIPLIER) / 2.0));
        } else if (numClusters == 2) {
            angle = angleBetween(cluster1Angle, cluster2Angle) <= 180 ? midAngleBetween(cluster1Angle, cluster2Angle) : midAngleBetween(cluster2Angle, cluster1Angle);
            size = 1 - cos(degreesToRadians(angleBetween(cluster1Angle, cluster2Angle) <= 180 ? angleBetween(cluster1Angle, cluster2Angle) / 2.0 : angleBetween(cluster2Angle, cluster1Angle) / 2.0));
        } else {
            double angleDiff12 = angleBetween(cluster1Angle, cluster2Angle);
            double angleDiff23 = angleBetween(cluster2Angle, cluster3Angle);
            double angleDiff31 = angleBetween(cluster3Angle, cluster1Angle);

            double biggestAngle = max(angleDiff12, max(angleDiff23, angleDiff31));

            if (angleDiff12 == biggestAngle) {
                angle = midAngleBetween(cluster2Angle, cluster1Angle);
                size = angleBetween(cluster2Angle, cluster1Angle) <= 180 ? 1 - cos(degreesToRadians(angleBetween(cluster2Angle, cluster1Angle) / 2.0)) : 1;
            } else if (angleDiff23 == biggestAngle) {
                angle = midAngleBetween(cluster3Angle, cluster2Angle);
                size = angleBetween(cluster3Angle, cluster2Angle) <= 180 ? 1 - cos(degreesToRadians(angleBetween(cluster3Angle, cluster2Angle) / 2.0)) : 1;
            } else {
                angle = midAngleBetween(cluster1Angle, cluster3Angle);
                size = angleBetween(cluster1Angle, cluster3Angle) <= 180 ? 1 - cos(degreesToRadians(angleBetween(cluster1Angle, cluster3Angle) / 2.0)) : 1;
            }
        }
    }
}

void LightSensorArray::resetStartEnds() {
    for (int i = 0; i < 4; i++) {
        starts[i] = LS_ES_DEFAULT;
        ends[i] = LS_ES_DEFAULT;
    }
}

double LightSensorArray::getLineAngle() {
    return angle;
}

double LightSensorArray::getLineSize() {
    return size;
}

uint16_t LightSensorArray::getFirst16Bit() {
    BitsUnion bitsUnion;
    bitsUnion.bits.b0 = data[0];
    bitsUnion.bits.b1 = data[1];
    bitsUnion.bits.b2 = data[2];
    bitsUnion.bits.b3 = data[3];
    bitsUnion.bits.b4 = data[4];
    bitsUnion.bits.b5 = data[5];
    bitsUnion.bits.b6 = data[6];
    bitsUnion.bits.b7 = data[7];
    bitsUnion.bits.b8 = data[8];
    bitsUnion.bits.b9 = data[9];
    bitsUnion.bits.b10 = data[10];
    bitsUnion.bits.b11 = data[11];
    bitsUnion.bits.b12 = data[12];
    bitsUnion.bits.b13 = data[13];
    bitsUnion.bits.b14 = data[14];
    bitsUnion.bits.b15 = data[15];

    return bitsUnion.bit16;
}

uint16_t LightSensorArray::getSecond16Bit() {
    BitsUnion bitsUnion;
    bitsUnion.bits.b0 = data[16];
    bitsUnion.bits.b1 = data[17];
    bitsUnion.bits.b2 = data[18];
    bitsUnion.bits.b3 = data[19];
    bitsUnion.bits.b4 = data[20];
    bitsUnion.bits.b5 = data[21];
    bitsUnion.bits.b6 = data[22];
    bitsUnion.bits.b7 = data[23];

    return bitsUnion.bit16;
}
