#ifndef MOTOR_ARRAY_H
#define MOTOR_ARRAY_H

#include <Arduino.h>
#include <Pins.h>
#include <Common.h>
#include <Motor.h>
#include <Config.h>

// Class for controlling the 4 motors in an omnidrive
class MotorArray {
public:
    // Motor objects
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

#endif // MOTOR_ARRAY_H
