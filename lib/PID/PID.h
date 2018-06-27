#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
public:
    // PID constants
    double kp;
    double ki;
    double kd;

    PID() {}
    PID(double p, double i, double d, double absoluteMax = 0.0);

    double update(double input, double setpoint, double modulus = 0.0);

private:
    unsigned long lastTime = micros(); // Time since last update. for I and D

    double absMax; // Absolute maxmimum correction

    double integral = 0;
    double lastError = 0; // Last error
};

#endif
