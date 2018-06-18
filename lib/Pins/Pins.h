#include <Arduino.h>

// --- Light Sensors --- //

#define LS0 A22
#define LS1 A1
#define LS2 A2
#define LS3 A3
#define LS_MUX A4

#define MUX_EN 46
#define MUX_WR 39
#define MUX_CS 38
#define MUX_A0 33
#define MUX_A1 34
#define MUX_A2 35
#define MUX_A3 36
#define MUX_A4 37

// --- Motors --- //

#define ENCODER_L_A 32
#define ENCODER_L_B 33
#define ENCODER_R_A 34
#define ENCODER_R_B 35
#define ENCODER_BL_A 36
#define ENCODER_BL_B 37
#define ENCODER_BR_A 38
#define ENCODER_BR_B 39

#define MOTOR_LEFT_PWM 2
#define MOTOR_LEFT_INA 6
#define MOTOR_LEFT_INB 23
#define MOTOR_LEFT_ENA 24
#define MOTOR_LEFT_ENB 28
#define MOTOR_LEFT_CS 16

#define MOTOR_RIGHT_PWM 3
#define MOTOR_RIGHT_INA 7
#define MOTOR_RIGHT_INB 22
#define MOTOR_RIGHT_ENA 25
#define MOTOR_RIGHT_ENB 29
#define MOTOR_RIGHT_CS 17

#define MOTOR_BACKLEFT_PWM 4
#define MOTOR_BACKLEFT_INA 8
#define MOTOR_BACKLEFT_INB 21
#define MOTOR_BACKLEFT_ENA 26
#define MOTOR_BACKLEFT_ENB 30
#define MOTOR_BACKLEFT_CS 18

#define MOTOR_BACKRIGHT_PWM 5
#define MOTOR_BACKRIGHT_INA 9
#define MOTOR_BACKRIGHT_INB 20
#define MOTOR_BACKRIGHT_ENA 27
#define MOTOR_BACKRIGHT_ENB 31
#define MOTOR_BACKRIGHT_CS 19

// --- Debug --- //

#define DEBUG_LED_RED 4
#define DEBUG_LED_GREEN 5
#define DEBUG_LED_BLUE 6
#define DEBUG_LED_ORANGE 7
#define DEBUG_LED_YELLOW 8
#define DEBUG_LED_WHITE 9

#define DEBUG_LED_ATTACKER 2
#define DEBUG_LED_DEFENDER 3

#define DEBUG_RGB_LED 23

// --- Screen --- //

#define SCREEN_YP A3
#define SCREEN_XM A2
#define SCREEN_YM 18
#define SCREEN_XP 15

#define BATTERY_VOLTAGE A22

// --- SPI --- //

#define MASTER_MOSI MOSI
#define MASTER_MISO MISO
#define MASTER_SCK ALT_SCK
#define MASTER_CS_SENSOR CS1
#define MASTER_CS_DEBUG CS4
#define MASTER_CS_MOTOR CS0

#define SLAVE_SENSOR_MOSI MOSI
#define SLAVE_SENSOR_MISO MISO
#define SLAVE_SENSOR_SCK ALT_SCK
#define SLAVE_SENSOR_CS CS0

#define SLAVE_DEBUG_MOSI MOSI
#define SLAVE_DEBUG_MISO MISO
#define SLAVE_DEBUG_SCK ALT_SCK
#define SLAVE_DEBUG_CS CS0

#define SLAVE_MOTOR_MOSI MOSI
#define SLAVE_MOTOR_MISO MISO
#define SLAVE_MOTOR_SCK ALT_SCK
#define SLAVE_MOTOR_CS CS0

// --- IR --- //

#define TSOP_0 0
#define TSOP_1 1
#define TSOP_2 2
#define TSOP_3 3
#define TSOP_4 4
#define TSOP_5 5
#define TSOP_6 6
#define TSOP_7 7
#define TSOP_8 8
#define TSOP_9 9
#define TSOP_10 19
#define TSOP_11 20
#define TSOP_12 21
#define TSOP_13 22
#define TSOP_14 23
#define TSOP_15 24
#define TSOP_16 25
#define TSOP_17 26
#define TSOP_18 27
#define TSOP_19 28
#define TSOP_20 29
#define TSOP_21 30
#define TSOP_22 31
#define TSOP_23 32

#define TSOP_PWR_1 43
#define TSOP_PWR_2 44
#define TSOP_PWR_3 45
#define TSOP_PWR_4 47
#define TSOP_PWR_5 48
#define TSOP_PWR_6 49
#define TSOP_PWR_7 50

// --- IMU --- //

#define IMU_1_CS 43
#define IMU_2_CS 55
#define IMU_3_CS 54
