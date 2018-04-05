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

    digitalWrite(MUX_WR, LOW);

    digitalWrite(MUX_A0, channel & 0x1);
    digitalWrite(MUX_A1, (channel >> 1) & 0x1);
    digitalWrite(MUX_A2, (channel >> 2) & 0x1);
    digitalWrite(MUX_A3, (channel >> 3) & 0x1);
    digitalWrite(MUX_A4, (channel >> 4) & 0x1);

    digitalWrite(MUX_WR, HIGH);
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
