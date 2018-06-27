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

    // PWM frequency
    frequency(457.7635);

    lastTimeA = micros();
    lastTimeB = micros();

    lastPIDTime = micros();
}

void Motor::update() {
    // Update encoder
    updateEncoderRPM();

    // Update PID

    if (rpmSetpoint == 0) {
        // If the rpm is 0, set PWM to 0 and ignore PID
        pwm = 0;
    } else {
        // Update PID only if the time since last PID update is greater than the time between two RPM reads
        unsigned long currentTime = micros();
        if (currentTime - lastPIDTime > (1.0 / (((double)abs(rpmSetpoint) / 60.0) * 24.0)) * 1000000.0) {
            pwm = constrain(abs(pwm) + rpmPID.update(rpm, abs(rpmSetpoint)), 0, MAX_PWM) * sign(rpmSetpoint);
            lastPIDTime = currentTime;
        }
    }

    // Set the controller outputs according to datasheet logic table (controller is VNH5050A)
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
    // Set desired RPM
    rpmSetpoint = value;
}

void Motor::brake() {
    // Stop the motor
    move(0);
}

void Motor::frequency(double frequency) {
    // Set the PWM frequency
    analogWriteFrequency(pwmPin, frequency);
}

void Motor::updateEncoderRPM() {
    // Read encoder

    uint8_t valueA = digitalRead(encAPin);
    uint8_t valueB = digitalRead(encBPin);

    unsigned long currentTime = micros();

    if (valueA != previousValueA) {
        // Hall sensor A state has changed
        rpm = 1.0 / (12.0 * ((double)(currentTime - lastTimeA) / 1000000.0 / 60.0));
        lastTimeA = currentTime;
        previousValueA = valueA;
    } else if (valueB != previousValueB) {
        // Hall sensor B state has changed
        rpm = 1.0 / (12.0 * ((double)(currentTime - lastTimeB) / 1000000.0 / 60.0));
        lastTimeB = currentTime;
        previousValueB = valueB;
    } else if (currentTime - lastTimeA > ENCODER_UPDATE_TIME || currentTime - lastTimeB > ENCODER_UPDATE_TIME || rpmSetpoint == 0) {
        // Neither sensor has changed within a certain time. RPM is approximately 0
        rpm = 0;
    }
}

double Motor::getRPM() {
    return rpm;
}
