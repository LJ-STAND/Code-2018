#include "Motor.h"

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

void Motor::encoderInterrupt() {
    counts++;
}

void Motor::move() {
    pwm = constrain(pwm + rpmPID.update(getRPM(), rpmSetpoint), 0, MAX_PWM) * sign(rpmSetpoint);

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

void Motor::setRPM(int rpm) {
    rpmSetpoint = rpm;
}

void Motor::brake() {
    setRPM(0);
}

void Motor::frequency(int frequency) {
    analogWriteFrequency(pwmPin, frequency);
}

int Motor::getRPM() {
    unsigned long currentTime = micros();
    return ((double)counts / 12.0) / ((double)(micros() - lastTime) / 1000000.0 / 60.0);
    lastTime = currentTime;
    counts = 0;
}
