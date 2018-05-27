#include "Camera.h"

void Camera::init() {
    Serial3.begin(9600);
}

void Camera::update() {
    while (Serial3.available() >= CAMERA_PACKET_SIZE) {
        uint8_t first = Serial3.read();

        if (first == CAMERA_START_BYTE) {
            newCameraData = true;

            uint8_t dataBuffer[CAMERA_PACKET_SIZE - 1];

            for (int i = 0; i < CAMERA_PACKET_SIZE - 1; i++) {
                dataBuffer[i] = Serial3.read();
            }

            yellowAngle = (dataBuffer[0] << 7) | dataBuffer[1];
            yellowPixelDistance = (dataBuffer[2] << 7) | dataBuffer[3];
            blueAngle = (dataBuffer[4] << 7) | dataBuffer[5];
            bluePixelDistance = (dataBuffer[6] << 7) | dataBuffer[7];
        }
    }
}

double Camera::centimeterDistance(int pixelDistance) {
    return 10.205 * pow(MATH_E, 0.037 * (double)pixelDistance);
}

double Camera::blueCentimeterDistance() {
    return centimeterDistance(bluePixelDistance);
}

double Camera::yellowCentimeterDistance() {
    return centimeterDistance(yellowPixelDistance);
}

double Camera::shortestDistance() {
    if (goalsVisible()) {
        if (!yellowGoalVisible()) {
            return blueCentimeterDistance();
        } else if (!blueGoalVisible()) {
            return yellowCentimeterDistance();
        } else {
            return min(blueCentimeterDistance(), yellowCentimeterDistance());
        }
    } 

    return 0;
}

bool Camera::yellowGoalVisible() {
    return yellowAngle != NO_GOAL_ANGLE;
}

bool Camera::blueGoalVisible() {
    return blueAngle != NO_GOAL_ANGLE;
}

bool Camera::goalsVisible() {
    return yellowGoalVisible() || blueGoalVisible();
}

bool Camera::newData() {
    bool returnValue = newCameraData;
    newCameraData = false;

    return returnValue;
}