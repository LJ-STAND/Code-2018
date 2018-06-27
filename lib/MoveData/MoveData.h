#ifndef MOVE_DATA_H
#define MOVE_DATA_H

// Movement data (angle, rotation, speed)
typedef struct MoveData {
    uint16_t angle = 0;
    int8_t speed = 0, rotation = 0;

    MoveData() {}
    MoveData(int moveAngle, int moveSpeed, int moveRotation) {
        angle = moveAngle;
        speed = moveSpeed;
        rotation = moveRotation;
    }
} MoveData;

#endif // MOVE_DATA_H
