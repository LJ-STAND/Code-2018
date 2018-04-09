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

    uint8_t previousValue = 0;
    unsigned long lastTime = 0;

    PID rpmPID = PID(MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);

    int pwm = 0;
    double rpm = 0.0;
    int rpmSetpoint;
};

#endif // MOTOR_H
