#include "MotorArray.h"

MotorArray::MotorArray() {
    motorLeft = Motor(MOTOR_LEFT_PWM, MOTOR_LEFT_INA, MOTOR_LEFT_INB, MOTOR_LEFT_ENA, MOTOR_LEFT_ENB, ENCODER_L_A, ENCODER_L_B);
    motorRight = Motor(MOTOR_RIGHT_PWM, MOTOR_RIGHT_INA, MOTOR_RIGHT_INB, MOTOR_RIGHT_ENA, MOTOR_RIGHT_ENB, ENCODER_R_A, ENCODER_R_B);
    motorBackLeft = Motor(MOTOR_BACKLEFT_PWM, MOTOR_BACKLEFT_INA, MOTOR_BACKLEFT_INB, MOTOR_BACKLEFT_ENA, MOTOR_BACKLEFT_ENB, ENCODER_BL_A, ENCODER_BL_B);
    motorBackRight = Motor(MOTOR_BACKRIGHT_PWM, MOTOR_BACKRIGHT_INA, MOTOR_BACKRIGHT_INB, MOTOR_BACKRIGHT_ENA, MOTOR_BACKRIGHT_ENB, ENCODER_BR_A, ENCODER_BR_B);
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
    // Compute each motor speed from an overall angle, rotation and speed

    // Convert speed and rotation from percentage to RPM
    int speedRPM = min(speed * MAX_SPEED / 100.0, MAX_SPEED);
    int rotationRPM = constrain(rotation * MAX_SPEED / 100.0, -MAX_SPEED, MAX_SPEED);

    // Compute each motor as a ratio based on angle
    double a = cos(degreesToRadians(angle)) * MOTOR_MULTIPLIER;
    double b = sin(degreesToRadians(angle)) * MOTOR_MULTIPLIER;

    double motorRightValue = a - b;
    double motorBackRightValue = a + b;
    double motorBackLeftValue = b - a;
    double motorLeftValue = -a - b;

    // Multiply by speed
    double maxSpeed = doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftValue), doubleAbs(motorRightValue)), doubleAbs(motorBackRightValue)), doubleAbs(motorBackLeftValue)));
    double updatedSpeed = maxSpeed != 0 ? (double)speedRPM / maxSpeed : 0;

    // Add rotation and ensure speeds are scaled to be less than max

    double motorRightSpeed = motorRightValue * updatedSpeed - rotationRPM;
    double motorLeftSpeed = motorLeftValue * updatedSpeed - rotationRPM;
    double motorBackRightSpeed = motorBackRightValue * updatedSpeed - rotationRPM;
    double motorBackLeftSpeed = motorBackLeftValue * updatedSpeed - rotationRPM;

    double maxSpeed2 = doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftSpeed), doubleAbs(motorRightSpeed)), doubleAbs(motorBackRightSpeed)), doubleAbs(motorBackLeftSpeed)));
    double updatedSpeed2 = maxSpeed2 != 0 ? (double)MAX_SPEED / maxSpeed2 : 0;

    if (updatedSpeed2 < 1) {
        motorRightSpeed = motorRightSpeed * updatedSpeed2;
        motorLeftSpeed = motorLeftSpeed * updatedSpeed2;
        motorBackRightSpeed = motorBackRightSpeed * updatedSpeed2;
        motorBackLeftSpeed = motorBackLeftSpeed * updatedSpeed2;
    }

    motorRight.move(round(motorRightSpeed));
    motorLeft.move(round(motorLeftSpeed));
    motorBackRight.move(round(motorBackRightSpeed));
    motorBackLeft.move(round(motorBackLeftSpeed));
}

void MotorArray::brake() {
    // Stop all motors
    
    motorRight.brake();
    motorLeft.brake();
    motorBackRight.brake();
    motorBackLeft.brake();
}
