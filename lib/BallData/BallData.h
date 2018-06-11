#ifndef BALL_DATA_H
#define BALL_DATA_H

#include <Config.h>
#include <Point.h>
#include <Common.h>

class BallData {
public:
    uint16_t angle;
    uint8_t strength;

    BallData() {
        angle = TSOP_NO_BALL;
        strength = 0;
    }

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

    Point position(int heading = 0) {
        Point returnPoint;

        returnPoint.setAngleMagnitude(doubleMod(angle + heading, 360), max(-0.9801 * (double)strength + 229.42, 1));

        return returnPoint;
    }

    double strengthFactor() {
        return constrain(((double)strength - (double)BALL_FAR_STRENGTH) / ((double)BALL_CLOSE_STRENGTH - BALL_FAR_STRENGTH), 0, 1);
    }
};

#endif // BALL_DATA_H
