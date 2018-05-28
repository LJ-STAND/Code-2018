#ifndef MOTOR_ARRAY_H
#define MOTOR_ARRAY_H

#include <Arduino.h>
#include <Pins.h>
#include <Common.h>
#include <Motor.h>
#include <Config.h>

class MotorArray {
public:
    Motor motorRight;
    Motor motorLeft;
    Motor motorBackRight;
    Motor motorBackLeft;

    MotorArray();
    void init();
    void move(int angle, int rotation, int speed);
    void update();
    void brake();

private:
    double lastLeftSpeed = 0;
    double lastRightSpeed = 0;
    double lastBackLeftSpeed = 0;
    double lastBackRightSpeed = 0;

    unsigned long lastTime = 0;
};

#endif // MOTOR_ARRAY_H
