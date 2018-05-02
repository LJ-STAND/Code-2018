#include "MotorArray.h"

MotorArray::MotorArray() {
    motorLeft = Motor(MOTOR_LEFT_PWM, MOTOR_LEFT_INA, MOTOR_LEFT_INB, MOTOR_LEFT_ENA, MOTOR_LEFT_ENB);
    motorRight = Motor(MOTOR_RIGHT_PWM, MOTOR_RIGHT_INA, MOTOR_RIGHT_INB, MOTOR_RIGHT_ENA, MOTOR_RIGHT_ENB);
    motorBackLeft = Motor(MOTOR_BACKLEFT_PWM, MOTOR_BACKLEFT_INA, MOTOR_BACKLEFT_INB, MOTOR_BACKLEFT_ENA, MOTOR_BACKLEFT_ENB);
    motorBackRight = Motor(MOTOR_BACKRIGHT_PWM, MOTOR_BACKRIGHT_INA, MOTOR_BACKRIGHT_INB, MOTOR_BACKRIGHT_ENA, MOTOR_BACKRIGHT_ENB);
}

void MotorArray::init() {
    analogWriteResolution(16);

    motorLeft.init();
    motorRight.init();
    motorBackLeft.init();
    motorBackRight.init();
}

void MotorArray::update() {
    motorLeft.update();
    motorRight.update();
    motorBackLeft.update();
    motorBackRight.update();
}

void MotorArray::move(int angle, int rotation, int speed) {
    int speedRPM = min(speed * MAX_SPEED / 100.0, MAX_SPEED);
    int rotationRPM = min(rotation * MAX_SPEED / 100.0, MAX_SPEED);

    double a = cos(degreesToRadians(angle)) * MOTOR_MULTIPLIER;
    double b = sin(degreesToRadians(angle)) * MOTOR_MULTIPLIER;

    double motorRightValue = a - b;
    double motorBackRightValue = a + b;
    double motorBackLeftValue = b - a;
    double motorLeftValue = -a - b;

    double updatedSpeed = (double) speedRPM / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftValue), doubleAbs(motorRightValue)), doubleAbs(motorBackRightValue)), doubleAbs(motorBackLeftValue)));

    int motorRightSpeed = (int) round(motorRightValue * updatedSpeed) - rotationRPM;
    int motorLeftSpeed = (int) round(motorLeftValue * updatedSpeed) - rotationRPM;
    int motorBackRightSpeed = (int) round(motorBackRightValue * updatedSpeed) - rotationRPM;
    int motorBackLeftSpeed = (int) round(motorBackLeftValue * updatedSpeed) - rotationRPM;

    double updatedSpeed2 = (double) MAX_SPEED / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftSpeed), doubleAbs(motorRightSpeed)), doubleAbs(motorBackRightSpeed)), doubleAbs(motorBackLeftSpeed)));

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

void MotorArray::brake() {
    motorRight.brake();
    motorLeft.brake();
    motorBackRight.brake();
    motorBackLeft.brake();
}
