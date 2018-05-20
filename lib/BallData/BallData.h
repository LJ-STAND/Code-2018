#ifndef BALL_DATA_H
#define BALL_DATA_H

#include <Config.h>

typedef struct BallData {
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
} BallData;

#endif // BALL_DATA_H
