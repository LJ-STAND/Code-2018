#include "Camera.h"

void Camera::init() {
    Serial3.begin(9600);
}

void Camera::update() {
    while (Serial3.available() >= CAMERA_PACKET_SIZE) {
        uint8_t first = Serial3.read();

        if (first == CAMERA_START_BYTE) {
            uint8_t dataBuffer[CAMERA_PACKET_SIZE - 1];

            for (int i = 0; i < CAMERA_PACKET_SIZE - 1; i++) {
                dataBuffer[i] = Serial3.read();
            }

            yellowAngle = (dataBuffer[0] << 7) | dataBuffer[1];
            yellowDistance = (dataBuffer[2] << 7) | dataBuffer[3];
            blueAngle = (dataBuffer[4] << 7) | dataBuffer[5];
            blueDistance = (dataBuffer[5] << 7) | dataBuffer[6];
        }
    }
}