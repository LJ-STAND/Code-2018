#include "Motor.h"

Motor::Motor(int pwm, int inA, int inB, int enA, int enB) {
    pwmPin = pwm;
    inAPin = inA;
    inBPin = inB;
    enAPin = enA;
    enBPin = enB;

    pinMode(pwm, OUTPUT);
    pinMode(inA, OUTPUT);
    pinMode(inB, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    digitalWrite(enAPin, HIGH);
    digitalWrite(enBPin, HIGH);
}

void Motor::move(int speed) {
    if (speed > 0) {
        analogWrite(pwmPin, constrain(speed, 0, 255));

        digitalWrite(inAPin, HIGH);
        digitalWrite(inBPin, LOW);
    } else if (speed < 0) {
        analogWrite(pwmPin, constrain(-speed, 0, 255));

        digitalWrite(inAPin, LOW);
        digitalWrite(inBPin, HIGH);
    } else {
        digitalWrite(pwmPin, HIGH);
        digitalWrite(inAPin, HIGH);
        digitalWrite(inBPin, HIGH);
    }
}

void Motor::brake() {
    move(0);
}

void Motor::frequency(int frequency) {
    analogWriteFrequency(pwmPin, frequency);
}
