#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <Config.h>

class Camera {
public:
    void init();
    void update();

    int blueCentimeterDistance();
    int yellowCentimeterDistance();

    int blueAngle;
    int bluePixelDistance;
    int yellowAngle;
    int yellowPixelDistance;
};

#endif // CAMERA_H
