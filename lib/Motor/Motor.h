#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Pins.h>
#include <Config.h>
#include <PID.h>
#include <Common.h>

// Motor class, manages encoder and speed control
class Motor {
public:
    Motor() {}
    Motor(int pwm, int inA, int inB, int enA, int enB, int encA, int encB);

    void init();
    void move(int value);
    void update();
    void frequency(double frequency);
    void brake();
    double getRPM();
    void updateEncoderRPM();

private:
    // Pins for controller chip and hall sensor inputs
    int pwmPin;
    int inAPin;
    int inBPin;
    int enAPin;
    int enBPin;
    int encAPin;
    int encBPin;

    uint8_t previousValueA = 0; // Previous state of hall sensor A
    uint8_t previousValueB = 0; // Previous state of hall sensor B

    unsigned long lastTimeA = 0; // Last state change time of hall sensor A
    unsigned long lastTimeB = 0; // Last state change time of hall sensor B

    PID rpmPID = PID(MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD); // PID controller for RPM

    unsigned long lastPIDTime = 0; // Time counter so PID is only updated as often as new rpm values are available

    int pwm = 0; // PWM value
    double rpm = 0.0; // Encoder RPM
    int rpmSetpoint; // Desired RPM
};

#endif // MOTOR_H
