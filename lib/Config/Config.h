/*
 * Header file to define configurable values within the whole project
 *(e.g. ORBIT_SPEED)
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- Light Sensors --- //

#define LS_NUM 36
#define LS_NUM_MULTIPLIER 10 // Equal to 360 / LS_NUM

#define LS_CALIBRATION_COUNT 10

#define LS_CALIBRATION_BUFFER 30

#define LS_ES_DEFAULT 100

// --- Motors --- //

#define MOTOR_PID_KP 0.5
#define MOTOR_PID_KI 0.0
#define MOTOR_PID_KD 0.02

#define MAX_PWM 65535

#define MOTOR_MULTIPLIER 707106781186547572737310929369 // equal to sin/cos(45)

#define MAX_SPEED 1000

#endif // CONFIG_H
