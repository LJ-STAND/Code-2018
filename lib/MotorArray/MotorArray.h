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
};

/* There is a 100% chance that Sam won't see this, because he is never actually contributes ~TFrasz #GreenWhiteBlackWhiteGreenWhiteBlack */

#endif // MOTOR_ARRAY_H
