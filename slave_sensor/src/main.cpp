#include <Arduino.h>
#include <Pins.h>
#include <LightSensorArray.h>
#include <Common.h>

LightSensorArray lightSensors;

void setup() {
    Serial.begin(9600);
    lightSensors.init();
}

void loop() {
    for (int i = 0; i < LS_NUM; i++) {
        Serial.print(lightSensors.readSensor(i));
        Serial.print(", ");
    }
    Serial.println();
}
