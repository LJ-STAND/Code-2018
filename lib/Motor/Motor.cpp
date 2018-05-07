#include "Motor.h"

Motor::Motor(int pwm, int inA, int inB, int enA, int enB, int encA, int encB) {
    pwmPin = pwm;
    inAPin = inA;
    inBPin = inB;
    enAPin = enA;
    enBPin = enB;
    encAPin = encA;
    encBPin = encB;
}

void Motor::init() {
    pinMode(pwmPin, OUTPUT);
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    pinMode(enAPin, OUTPUT);
    pinMode(enBPin, OUTPUT);
    pinMode(encAPin, INPUT);
    pinMode(encBPin, INPUT);

    digitalWrite(enAPin, HIGH);
    digitalWrite(enBPin, HIGH);

    frequency(19000);

    lastTimeA = micros();
    lastTimeB = micros();

    lastPIDTime = micros();
}

void Motor::update() {
    updateEncoderRPM();

    if (rpmSetpoint == 0) {
        pwm = 0;
    } else {
        unsigned long currentTime = micros();
        if (currentTime - lastPIDTime > (1.0 / (((double)abs(rpmSetpoint) / 60.0) * 24.0)) * 1000000.0) {
            pwm = constrain(abs(pwm) + rpmPID.update(rpm, abs(rpmSetpoint)), 0, MAX_PWM) * sign(rpmSetpoint);
            lastPIDTime = currentTime;
        }
    }

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
    uint8_t valueA = digitalRead(encAPin);
    uint8_t valueB = digitalRead(encBPin);

    unsigned long currentTime = micros();

    if (valueA != previousValueA) {
        rpm = 1.0 / (12.0 * ((double)(currentTime - lastTimeA) / 1000000.0 / 60.0));
        lastTimeA = currentTime;
        previousValueA = valueA;
    } else if (valueB != previousValueB) {
        rpm = 1.0 / (12.0 * ((double)(currentTime - lastTimeB) / 1000000.0 / 60.0));
        lastTimeB = currentTime;
        previousValueB = valueB;
    } else if (currentTime - lastTimeA > ENCODER_UPDATE_TIME || currentTime - lastTimeB > ENCODER_UPDATE_TIME || rpmSetpoint == 0) {
        rpm = 0;
    }
}

double Motor::getRPM() {
    return rpm;
}
