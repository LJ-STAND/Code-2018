#include "PID.h"

PID::PID(double p, double i, double d, double absoluteMax) {
    kp = p;
    ki = i;
    kd = d;

    absMax = absoluteMax;

    lastTime = micros();
}

double PID::update(double input, double setpoint, double modulus) {
    // Compute error and correction.
    // Modulus ensures that for values with modulus (e.g. heading) the difference is the smallest difference between two values.
    // e.g. for mod 360, a value of 350 gives an error of 10 not 350.

    double derivative;
    double error = setpoint - input;

    unsigned long currentTime = micros();
    double elapsedTime = (currentTime - lastTime) / 1000000.0;
    lastTime = currentTime;

    integral += elapsedTime * error;

    // Compute the derivative on the difference in error to avoid derivative kick
    
    if (modulus != 0.0) {
        double difference = (error - lastError);

        if (difference < -modulus) {
            difference += modulus;
        } else if (difference > modulus) {
            difference -= modulus;
        }

        derivative = -difference / elapsedTime;
    } else {
        derivative = -(error - lastError) / elapsedTime;
    }

    lastError = error;

    double correction = kp * error + ki * integral - kd * derivative;

    // Constrain the correction
    return absMax == 0 ? correction : constrain(correction, -absMax, absMax);
}
