#include <Motor.h>
#include <Arduino.h>
#include <Pins.h>

Motor motorLeft(MOTOR_LEFT_PWM, MOTOR_LEFT_INA, MOTOR_LEFT_INB, MOTOR_LEFT_ENA, MOTOR_LEFT_ENB);
Motor motorRight(MOTOR_RIGHT_PWM, MOTOR_RIGHT_INA, MOTOR_RIGHT_INB, MOTOR_RIGHT_ENA, MOTOR_RIGHT_ENB);
Motor motorBackLeft(MOTOR_BACKLEFT_PWM, MOTOR_BACKLEFT_INA, MOTOR_BACKLEFT_INB, MOTOR_BACKLEFT_ENA, MOTOR_BACKLEFT_ENB);
Motor motorBackRight(MOTOR_BACKRIGHT_PWM, MOTOR_BACKRIGHT_INA, MOTOR_BACKRIGHT_INB, MOTOR_BACKRIGHT_ENA, MOTOR_BACKRIGHT_ENB);


void setup() {
    motorLeft.frequency(19000);
    pinMode(MOTOR_LEFT_CS, INPUT);
}

void loop() {
    // for (int i = 255; i > 0; i--) {
    //     motorLeft.move(i);
    //     motorRight.move(i);
    //     motorBackLeft.move(i);
    //     motorBackRight.move(i);
    //     delay(50);
    // }

    motorLeft.move(255);

    Serial.println(analogRead(MOTOR_LEFT_CS));

    // motorLeft.move(255);
    // motorRight.move(255);
    // motorBackLeft.move(255);
    // motorBackRight.move(255);
    //
    // delay(1000);
    //
    // motorLeft.move(0);
    // motorRight.move(0);
    // motorBackLeft.move(0);
    // motorBackRight.move(0);
    //
    // delay(1000);
}
