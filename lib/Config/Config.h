/*
 * Header file to define configurable values within the whole project
 *(e.g. ORBIT_SPEED)
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- Light Sensors --- //

#define LS_NUM 36
#define LS_NUM_MULTIPLIER 10 // = 360 / LS_NUM

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

#define MOTOR_MULTIPLIER 0.707106781186547572737310929369 // = sin&cos(45)

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

#define MAX_SPEED 600

// --- IMU --- //

#define IMU_CALIBRATION_COUNT 20
#define IMU_CALIBRATION_TIME 50

// --- Orbit --- //

#define ORBIT_SHORT_STRENGTH 100
#define ORBIT_BIG_STRENGTH 80

#define ORBIT_SMALL_ANGLE 5
#define ORBIT_BIG_ANGLE 60

#define ORBIT_BALL_FORWARD_ANGLE_TIGHTENER 0.5

#define ORBIT_SPEED 100

#define BALL_FRONT_BUFFER 10

// --- Debug --- //

#define LED_BLINK_TIME_MASTER 100000
#define LED_BLINK_TIME_SLAVE_MOTOR 200000
#define LED_BLINK_TIME_SLAVE_SENSOR 400000
#define LED_BLINK_TIME_SLAVE_DEBUG 800000

#define RGB_LED_COUNT 12

#define BATTERY_VOLTAGE_MULTIPILER 0.01767938931
#define MIN_BATTERY_VOLTAGE 8.00
#define MAX_BATTERY_VOLTAGE 12.6

// --- Screen --- //

#define TS_MINX 150
#define TS_MINY 90
#define TS_MAXX 905
#define TS_MAXY 890

#define TS_RESISTANCE 300

#define MIN_PRESSURE 100

#define	BLACK 0x0000
#define	BLUE 0x001F
#define	RED 0xF800
#define	GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#endif // CONFIG_H
