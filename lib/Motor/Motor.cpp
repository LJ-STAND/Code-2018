#include "Motor.h"

Motor::Motor(int pwm, int inA, int inB, int enA, int enB, int encoder) {
    pwmPin = pwm;
    inAPin = inA;
    inBPin = inB;
    enAPin = enA;
    enBPin = enB;
    encoderPin = encoder;
}

void Motor::init() {
    pinMode(pwmPin, OUTPUT);
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    pinMode(enAPin, OUTPUT);
    pinMode(enBPin, OUTPUT);
    pinMode(encoderPin, INPUT);

    digitalWrite(enAPin, HIGH);
    digitalWrite(enBPin, HIGH);

    frequency(915.527);

    lastTime = micros();

    previousValue = digitalRead(encoderPin);
}

void Motor::update() {
    updateEncoderRPM();

    double pidError = rpmPID.update(rpm, abs(rpmSetpoint));
    pwm = constrain(abs(pwm) + pidError, 0, MAX_PWM) * sign(rpmSetpoint);

    if (pwm > 0) {
        analogWrite(pwmPin, constrain(pwm, 0, MAX_PWM));

        digitalWrite(inAPin, HIGH);
        digitalWrite(inBPin, LOW);
    } else if (pwm < 0) {
        analogWrite(pwmPin, constrain(-pwm, 0, MAX_PWM));

        digitalWrite(inAPin, LOW);
        digitalWrite(inBPin, HIGH);
    } else {
        digitalWrite(pwmPin, HIGH);
        digitalWrite(inAPin, HIGH);
        digitalWrite(inBPin, HIGH);
    }
}

void Motor::move(int value) {
    rpmSetpoint = value;
}

void Motor::brake() {
    move(0);
}

void Motor::frequency(int frequency) {
    analogWriteFrequency(pwmPin, frequency);
}

void Motor::updateEncoderRPM() {
    uint8_t value = digitalRead(encoderPin);

    if (value != previousValue) {
        unsigned long currentTime = micros();
        rpm = 1.0 / (12.0 * ((double)(micros() - lastTime) / 1000000.0 / 60.0));
        lastTime = currentTime;
        previousValue = value;
    } else if (micros() - lastTime > 100000) {
        rpm = 0;
    }
}

double Motor::getRPM() {
    return rpm;
}
