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
    Motor(int pwm, int inA, int inB, int enA, int enB, int encoder);

    void init();
    void move();
    void frequency(int frequency);
    void brake();
    double getRPM();
    void updateRPM();
    void setRPM(int value);

private:
    int pwmPin;
    int inAPin;
    int inBPin;
    int enAPin;
    int enBPin;
    int encoderPin;

    uint8_t previousValue;
    unsigned long lastTime;

    PID rpmPID;

    int pwm;
    double rpm = 0.0;
    int rpmSetpoint;
};

#endif // MOTOR_H
