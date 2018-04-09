#include "MotorArray.h"

MotorArray::MotorArray() {
    Motor motorLeft = Motor(MOTOR_LEFT_PWM, MOTOR_LEFT_INA, MOTOR_LEFT_INB, MOTOR_LEFT_ENA, MOTOR_LEFT_ENB, ENCODER_L_A);
    Motor motorRight = Motor(MOTOR_RIGHT_PWM, MOTOR_RIGHT_INA, MOTOR_RIGHT_INB, MOTOR_RIGHT_ENA, MOTOR_RIGHT_ENB, ENCODER_R_A);
    Motor motorBackLeft = Motor(MOTOR_BACKLEFT_PWM, MOTOR_BACKLEFT_INA, MOTOR_BACKLEFT_INB, MOTOR_BACKLEFT_ENA, MOTOR_BACKLEFT_ENB, ENCODER_BL_A);
    Motor motorBackRight = Motor(MOTOR_BACKRIGHT_PWM, MOTOR_BACKRIGHT_INA, MOTOR_BACKRIGHT_INB, MOTOR_BACKRIGHT_ENA, MOTOR_BACKRIGHT_ENB, ENCODER_BR_A);
}

void MotorArray::move(int angle, int rotation, int speed) {

    double a = cos(degreesToRadians(angle)) * MOTOR_MULTIPLIER;
    double b = sin(degreesToRadians(angle)) * MOTOR_MULTIPLIER;

    double motorRightValue = b - a;
    double motorBackRightValue = -a - b;
    double motorBackLeftValue = a - b;
    double motorLeftValue = a + b;

    double updatedSpeed = (double) speed / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftValue), doubleAbs(motorRightValue)), doubleAbs(motorBackRightValue)), doubleAbs(motorBackLeftValue)));

    int motorRightSpeed = (int) round(motorRightValue * updatedSpeed) + rotation;
    int motorLeftSpeed = (int) round(motorLeftValue * updatedSpeed) + rotation;
    int motorBackRightSpeed = (int) round(motorBackRightValue * updatedSpeed) + rotation;
    int motorBackLeftSpeed = (int) round(motorBackLeftValue * updatedSpeed) + rotation;

    double updatedSpeed2 = (double) 255 / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftSpeed), doubleAbs(motorRightSpeed)), doubleAbs(motorBackRightSpeed)), doubleAbs(motorBackLeftSpeed)));

    if (updatedSpeed2 < 1) {
        int motorRightSpeed = (int) round(motorRightSpeed * updatedSpeed2);
        int motorLeftSpeed = (int) round(motorLeftSpeed * updatedSpeed2);
        int motorBackRightSpeed = (int) round(motorBackRightSpeed * updatedSpeed2);
        int motorBackLeftSpeed = (int) round(motorBackLeftSpeed * updatedSpeed2);
    }

    motorRight.move(motorRightSpeed);
    motorLeft.move(motorLeftSpeed);
    motorBackRight.move(motorBackRightSpeed);
    motorBackLeft.move(motorBackLeftSpeed);
}

void MotorArray::move(MoveData data) {
    move(data.angle, data.rotation, data.speed);
}

void MotorArray::brake() {
    motorRight.brake();
    motorLeft.brake();
    motorBackRight.brake();
    motorBackLeft.brake();
}
