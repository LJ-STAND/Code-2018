#include "Slave.h"

void Slave::init(int csPin) {
    cs = csPin;

    spi.enableCS(cs, CS_ActiveLOW);
}

uint16_t Slave::transaction(SlaveCommand command) {
    dataOut[0] = (uint16_t)command;

    for (int i = 0; i < 3; i++) {
        spi.txrx16(dataOut, dataIn, 1, CTAR_0, cs);
    }

    return dataIn[0];
}

void SlaveTSOP::init() {
    Slave::init(CS1);
}

int SlaveTSOP::getTSOPAngle() {
    return transaction(SlaveCommand::tsopAngle);
}

int SlaveTSOP::getTSOPStrength() {
    return transaction(SlaveCommand::tsopStrength);
}

BallData SlaveTSOP::getBallData() {
    int angle = getTSOPAngle();
    int strength = getTSOPStrength();

    return BallData(angle, strength, angle != TSOP_NO_BALL);
}
