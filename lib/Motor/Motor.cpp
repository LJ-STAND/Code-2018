#include "Motor.h"

Motor::Motor(int pwm, int inA, int inB, int enA, int enB) {
    pwmPin = pwm;
    inAPin = inA;
    inBPin = inB;
    enAPin = enA;
    enBPin = enB;
}

void Motor::init() {
    pinMode(pwmPin, OUTPUT);
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    pinMode(enAPin, OUTPUT);
    pinMode(enBPin, OUTPUT);

    digitalWrite(enAPin, HIGH);
    digitalWrite(enBPin, HIGH);

    frequency(915.527);

    lastTime = micros();
}

void Motor::update() {
    if (lastTime > ENCODER_UPDATE_TIME || count > 0) {
        updateEncoderRPM();
    }

    Serial.println(rpm);

    pwm = constrain(abs(pwm) + rpmPID.update(rpm, abs(rpmSetpoint)), 0, MAX_PWM) * sign(rpmSetpoint);

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
    unsigned long currentTime = micros();
    rpm = (count / 24) / ((currentTime - lastTime) / (1000000 * 60));
    lastTime = currentTime;
    count = 0;
}

void Motor::updateEncoderCounts() {
    count++;
}

double Motor::getRPM() {
    return rpm;
}
