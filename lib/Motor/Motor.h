#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Pins.h>

class Motor {
public:
    Motor() {}
    Motor(int pwm, int inA, int inB, int enA, int enB);

    void move(int speed);
    void frequency(int frequency);
    void brake();

private:
    int pwmPin;
    int inAPin;
    int inBPin;
    int enAPin;
    int enBPin;
};

#endif
