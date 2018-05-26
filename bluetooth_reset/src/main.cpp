#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial5.begin(115200);
}

void loop() {
    if (Serial.available()) {
        Serial5.write(Serial.read());
    }

    if (Serial5.available()) {
        Serial.write(Serial5.read());
    }
}