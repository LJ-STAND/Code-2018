#include <Arduino.h>

void setup() {
    Serial.begin(9600);
    Serial5.begin(9600);
}

void loop() {
    if (Serial.available()) {
        Serial5.write(Serial.read());
    }

    if (Serial5.available()) {
        Serial.write(Serial5.read());
    }
}