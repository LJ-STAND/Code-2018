#ifndef BALL_DATA_H
#define BALL_DATA_H

#include <Config.h>
#include <Point.h>
#include <Common.h>

class BallData {
public:
    uint16_t angle;
    uint8_t strength;

    BallData() {}
    BallData(uint16_t angle, uint8_t strength) : angle(angle), strength(strength) {}

    bool visible() {
        return angle != TSOP_NO_BALL;
    }

    bool operator ==(const BallData &d) {
         return angle == d.angle && strength == d.strength;
    }

    bool operator !=(const BallData &d) {
        return angle != d.angle || strength != d.strength;
    }

    Point position(int heading) {
        Point returnPoint;

        returnPoint.setAngleMagnitude(doubleMod(angle + heading, 360), 180 - strength);

        return returnPoint;
    }
};

#endif // BALL_DATA_H
