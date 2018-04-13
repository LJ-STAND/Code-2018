/*
 * Header file to define configurable values within the whole project
 *(e.g. ORBIT_SPEED)
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- Light Sensors --- //

#define LS_NUM 36
#define LS_NUM_MULTIPLIER 10 // Equal to 360 / LS_NUM

#define LS_CALIBRATION_COUNT 50

#define LS_CALIBRATION_BUFFER 70

#define LS_ES_DEFAULT 100

#define NO_LINE_ANGLE 400
#define NO_LINE_SIZE 3

// --- Motors --- //

#define MOTOR_PID_KP 0.5
#define MOTOR_PID_KI 0.0
#define MOTOR_PID_KD 0.02

#define MAX_PWM 65535

#define MOTOR_MULTIPLIER 707106781186547572737310929369 // equal to sin/cos(45)

#define TSOP_NUM 24

#define TSOP_LOOP_COUNT 255

#define TSOP_UNLOCK_DELAY 2

#define TSOP_BEST_TSOP_NO_ANGLE 5
#define TSOP_BEST_TSOP_NO_STRENGTH 2

#define TSOP_K1 10
#define TSOP_K2 2
#define TSOP_K3 1

#define TSOP_MIN_IGNORE 10
#define TSOP_MAX_IGNORE 220

#define TSOP_FIRST_TSOP_WEIGHT 3
#define TSOP_SECOND_TSOP_WEIGHT 2

#define TSOP_FILTER_NOISE false
#define TSOP_FILTER_SURROUNDING true

#define TSOP_NO_BALL 400

#define TSOP_HAS_BALL_STRENGTH 130

#define DEBUG_TSOP false

#define MAX_SPEED 1500



// Orbit

#define ORBIT_SHORT_STRENGTH 100
#define ORBIT_BIG_STRENGTH 80

#define ORBIT_SMALL_ANGLE 5
#define ORBIT_BIG_ANGLE 60

#define ORBIT_BALL_FORWARD_ANGLE_TIGHTENER 0.5

#define ORBIT_SPEED 100

#define BALL_FRONT_BUFFER 10

#endif // CONFIG_H
