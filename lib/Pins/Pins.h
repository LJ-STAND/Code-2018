#include "Arduino.h"

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

#define MUX_EN_PORT // PTB21
#define MUX_WR_PORT // PTA17
#define MUX_CS_PORT // PTC11
#define MUX_A0_PORT // PTE24
#define MUX_A1_PORT // PTE25
#define MUX_A2_PORT // PTC8
#define MUX_A3_PORT // PTC9
#define MUX_A4_PORT // PTC10

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
#define MOTOR_BACKRIGHT_ENB 30
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

#define V_BAT A22
