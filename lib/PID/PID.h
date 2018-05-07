#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
public:
    double kp;
    double ki;
    double kd;

    PID() {}
    PID(double p, double i, double d, double absoluteMax = 0.0);

    double update(double input, double setpoint, double modulus = 0.0);

private:
    unsigned long lastTime = micros();

    double absMax;

    double integral = 0;
    double lastError = 0;
};

#endif
