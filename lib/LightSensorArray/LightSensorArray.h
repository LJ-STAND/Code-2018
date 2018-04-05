#include <Arduino.h>
#include <Pins.h>

class LightSensorArray {
public:
    void init();

    void changeMUXChannel(uint8_t channel);
    int readSensor(int sensor);

private:
    uint8_t muxChannels[32] = {12, 11, 10, 9, 13, 14, 15, 16, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 1, 2, 3, 4, 5, 6, 7, 8};
};
