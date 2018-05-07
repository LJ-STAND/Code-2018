#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Pins.h>
#include <Config.h>
#include <PID.h>
#include <Common.h>

class Motor {
public:
    Motor() {}
    Motor(int pwm, int inA, int inB, int enA, int enB, int encA, int encB);

    void init();
    void move(int value);
    void update();
    void frequency(int frequency);
    void brake();
    double getRPM();
    void updateEncoderRPM();

private:
    int pwmPin;
    int inAPin;
    int inBPin;
    int enAPin;
    int enBPin;
    int encAPin;
    int encBPin;

    uint8_t previousValueA = 0;
    uint8_t previousValueB = 0;

    unsigned long lastTimeA = 0;
    unsigned long lastTimeB = 0;

    PID rpmPID = PID(MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);

    unsigned long lastPIDTime = 0;

    int pwm = 0;
    double rpm = 0.0;
    int rpmSetpoint;
};

#endif // MOTOR_H
