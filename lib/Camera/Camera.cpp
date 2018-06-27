#include "Camera.h"

void Camera::init() {
    // Begin UART
    Serial3.begin(CAMERA_BAUD);
}

void Camera::update() {
    // Receive data from camera

    // Loops while the amount of data is >= the packet size
    while (Serial3.available() >= CAMERA_PACKET_SIZE) {
        uint8_t first = Serial3.read();

        // Makes sure the first byte is the start of packet indicator, otherwise keeps looping
        if (first == CAMERA_START_BYTE) {
            newCameraData = true;

            uint8_t dataBuffer[CAMERA_PACKET_SIZE - 1];

            // Put all the data into an array
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
    // Converts pixel distance on the camera to physical distance in cm
    return 10.205 * pow(MATH_E, 0.037 * (double)pixelDistance);
}

double Camera::blueCentimeterDistance() {
    // Converts the blue pixel distance to cm distance
    return centimeterDistance(bluePixelDistance);
}

double Camera::yellowCentimeterDistance() {
    // Converts the yellow pixel distance to cm distance
    return centimeterDistance(yellowPixelDistance);
}

double Camera::shortestDistance() {
    // Distance in cm of the closest goal

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
    // Yellow goal is visible or not. 400 is transmitted from the camera if not
    return yellowAngle != NO_GOAL_ANGLE;
}

bool Camera::blueGoalVisible() {
    // Blue goal is visible or not. 400 is transmitted from the camera if not
    return blueAngle != NO_GOAL_ANGLE;
}

bool Camera::goalsVisible() {
    // Either goal is visible
    return yellowGoalVisible() || blueGoalVisible();
}

bool Camera::newData() {
    // Flag for if new data has been recieved
    bool returnValue = newCameraData;
    newCameraData = false;

    return returnValue;
}

bool Camera::yellowClosest() {
    // Yellow goal is closest or not
    return shortestDistance() == yellowCentimeterDistance();
}