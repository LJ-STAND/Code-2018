#ifndef SLAVE_H
#define SLAVE_H

#include <t3spi.h>
#include <Pins.h>
#include <Config.h>
#include <BallData.h>

enum SlaveCommand: int {
    lineAngle,
    lineSize,
    lightSensorsFirst16Bit,
    lightSensorsSecond16Bit,
    tsopAngle,
    tsopStrength
};

class Slave {
public:
    void init(int csPin);
    uint16_t transaction(SlaveCommand command);

private:
    volatile uint16_t dataIn[1];
    volatile uint16_t dataOut[1];
    int cs;
};

class SlaveTSOP: public Slave {
public:
    void init();
    int getTSOPAngle();
    int getTSOPStrength();
    BallData getBallData();
};

#endif // SLAVE_H
