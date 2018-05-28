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

    lastTime = micros();
}

void MotorArray::update() {
    motorLeft.update();
    motorRight.update();
    motorBackLeft.update();
    motorBackRight.update();
}

void MotorArray::move(int angle, int rotation, int speed) {
    int speedRPM = min(speed * MAX_SPEED / 100.0, MAX_SPEED);
    int rotationRPM = constrain(rotation * MAX_SPEED / 100.0, -MAX_SPEED, MAX_SPEED);

    double a = cos(degreesToRadians(angle)) * MOTOR_MULTIPLIER;
    double b = sin(degreesToRadians(angle)) * MOTOR_MULTIPLIER;

    double motorRightValue = a - b;
    double motorBackRightValue = a + b;
    double motorBackLeftValue = b - a;
    double motorLeftValue = -a - b;

    double updatedSpeed = (double)speedRPM / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftValue), doubleAbs(motorRightValue)), doubleAbs(motorBackRightValue)), doubleAbs(motorBackLeftValue)));

    double motorRightSpeed = motorRightValue * updatedSpeed - rotationRPM;
    double motorLeftSpeed = motorLeftValue * updatedSpeed - rotationRPM;
    double motorBackRightSpeed = motorBackRightValue * updatedSpeed - rotationRPM;
    double motorBackLeftSpeed = motorBackLeftValue * updatedSpeed - rotationRPM;

    double updatedSpeed2 = (double)MAX_SPEED / doubleAbs(fmax(fmax(fmax(doubleAbs(motorLeftSpeed), doubleAbs(motorRightSpeed)), doubleAbs(motorBackRightSpeed)), doubleAbs(motorBackLeftSpeed)));

    if (updatedSpeed2 < 1) {
        motorRightSpeed = motorRightSpeed * updatedSpeed2;
        motorLeftSpeed = motorLeftSpeed * updatedSpeed2;
        motorBackRightSpeed = motorBackRightSpeed * updatedSpeed2;
        motorBackLeftSpeed = motorBackLeftSpeed * updatedSpeed2;
    }

    double leftDifference = motorLeftSpeed - lastLeftSpeed;
    double rightDifference = motorRightSpeed - lastRightSpeed;
    double backLeftDifference = motorBackLeftSpeed - lastBackLeftSpeed;
    double backRightDifference = motorBackRightSpeed - lastBackRightSpeed;
    
    double maxDifference = fmax(fmax(fmax(doubleAbs(leftDifference), doubleAbs(rightDifference)), doubleAbs(backLeftDifference)), doubleAbs(backRightDifference));

    unsigned long currentTime = micros();
    double acceleration = (double)MOTOR_MAX_ACCELERATION * (double)(currentTime - lastTime) / 1000000.0;

    lastTime = currentTime;

    double difference = fmin(maxDifference, acceleration);

    motorLeftSpeed = lastLeftSpeed + difference * (double)leftDifference / (double)maxDifference;
    motorRightSpeed = lastRightSpeed + difference * (double)rightDifference / (double)maxDifference;
    motorBackLeftSpeed = lastBackLeftSpeed + difference * (double)backLeftDifference / (double)maxDifference;
    motorBackRightSpeed = lastBackRightSpeed + difference * (double)backRightDifference / (double)maxDifference;

    lastLeftSpeed = motorLeftSpeed;
    lastRightSpeed = motorRightSpeed;
    lastBackLeftSpeed = motorBackLeftSpeed;
    lastBackRightSpeed = motorBackRightSpeed;

    motorRight.move(round(motorRightSpeed));
    motorLeft.move(round(motorLeftSpeed));
    motorBackRight.move(round(motorBackRightSpeed));
    motorBackLeft.move(round(motorBackLeftSpeed));
}

void MotorArray::brake() {
    motorRight.brake();
    motorLeft.brake();
    motorBackRight.brake();
    motorBackLeft.brake();
}
