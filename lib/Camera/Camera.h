#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <Config.h>
#include <Common.h>

// Camera class interfaces with the OpenMV over UART
class Camera {
public:
    void init();
    void update();

    double centimeterDistance(int pixelDistance);
    double blueCentimeterDistance();
    double yellowCentimeterDistance();
    double shortestDistance();
    bool yellowClosest();

    bool blueGoalVisible();
    bool yellowGoalVisible();
    bool goalsVisible();
    bool newData();

    // Note angles are relative to the front of the robot, and go clockwise from 0 to 360 degrees

    int blueAngle; // Angle of the blue goal
    int bluePixelDistance; // Pixel distance on the camera
    int yellowAngle; // Angle of the yellow goal
    int yellowPixelDistance; // Pixel distance on the camera

private:
    bool newCameraData;
};

#endif // CAMERA_H
