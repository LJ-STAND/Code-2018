#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <Config.h>
#include <Common.h>

class Camera {
public:
    void init();
    void update();

    double centimeterDistance(int pixelDistance);
    double blueCentimeterDistance();
    double yellowCentimeterDistance();
    double shortestDistance();

    bool blueGoalVisible();
    bool yellowGoalVisible();
    bool goalsVisible();
    bool newData();

    int blueAngle;
    int bluePixelDistance;
    int yellowAngle;
    int yellowPixelDistance;

private:
    bool newCameraData;
};

#endif // CAMERA_H
