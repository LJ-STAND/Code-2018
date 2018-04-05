#include <Arduino.h>
#include <Pins.h>
#include <LightSensorArray.h>

LightSensorArray lightSensors;

void setup() {
    Serial.begin(9600);
    lightSensors.init();
}

void loop() {
    for (int i = 0; i < 36; i++) {
        Serial.print(lightSensors.readSensor(i));
        Serial.print(", ");
    }
    Serial.println();
}
