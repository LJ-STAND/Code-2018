#ifndef BALL_DATA_H
#define BALL_DATA_H

typedef struct BallData {
    uint16_t angle;
    uint16_t strength;
    bool visible;

    BallData() {}
    BallData(int a, int s, bool v) : angle(a), strength(s), visible(v) {}
} BallData;

#endif // BALL_DATA_H
