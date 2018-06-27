#ifndef BALL_DATA_H
#define BALL_DATA_H

#include <Config.h>
#include <Point.h>
#include <Common.h>

// BallData class for holding the position information of the ball
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
        // Whether the sensors can see the ball. A value of 400 is transmitted over SPI if the ball is invisible
        return angle != TSOP_NO_BALL;
    }

    bool operator ==(const BallData &d) {
         return angle == d.angle && strength == d.strength;
    }

    bool operator !=(const BallData &d) {
        return angle != d.angle || strength != d.strength;
    }

    Point position(int heading = 0) {
        // Returns the position as a cartesian point
        Point returnPoint;

        // The function max(-0.9801x + 229.42, 1) is an approximation of strength (x) to distance in cm
        returnPoint.setAngleMagnitude(doubleMod(angle + heading, 360), max(-0.9801 * (double)strength + 229.42, 1));

        return returnPoint;
    }

    double strengthFactor() {
        // Returns a number from 0 to 1 indicating the relative strength. 0 is far away, 1 is close
        return constrain(((double)strength - (double)BALL_FAR_STRENGTH) / ((double)BALL_CLOSE_STRENGTH - BALL_FAR_STRENGTH), 0, 1);
    }
};

#endif // BALL_DATA_H
