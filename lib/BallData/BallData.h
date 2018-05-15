#ifndef BALL_DATA_H
#define BALL_DATA_H

typedef struct BallData {
    uint16_t angle;
    uint8_t strength;
    bool visible;

    BallData() {}
    BallData(uint16_t angle, uint8_t strength, bool visible) : angle(angle), strength(strength), visible(visible) {}
} BallData;

#endif // BALL_DATA_H
