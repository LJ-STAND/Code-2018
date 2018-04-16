#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <MovingAverage.h>

class Screen {
public:
    Screen() {}

    void init();
    void updateBatteryMeter();

private:
    TouchScreen ts = TouchScreen(SCREEN_XP, SCREEN_YP, SCREEN_XM, SCREEN_YM, TS_RESISTANCE);
    Adafruit_ILI9341_8bit tft;

    MovingAverage batteryAverage = MovingAverage(50);
};

#endif // SCREEN_H
