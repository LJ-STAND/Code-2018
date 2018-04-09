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
    Motor(int pwm, int inA, int inB, int enA, int enB) : pwmPin(pwm), inAPin(inA), inBPin(inB), enAPin(enA), enBPin(enB) {
        rpmPID = PID(MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);
    }

    void init();
    void move();
    void frequency(int frequency);
    void brake();
    int getRPM();
    void setRPM(int rpm);

    void encoderInterrupt();

private:
    int pwmPin;
    int inAPin;
    int inBPin;
    int enAPin;
    int enBPin;

    volatile int counts;
    unsigned long lastTime;

    PID rpmPID;

    int pwm;
    int rpmSetpoint;
};

#endif // MOTOR_H
