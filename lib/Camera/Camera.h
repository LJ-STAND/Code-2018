#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <Config.h>

class Camera {
public:
    void init();
    void update();

    int blueAngle;
    int blueDistance;
    int yellowAngle;
    int yellowDistance;
};

#endif // CAMERA_H
