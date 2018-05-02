#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <MovingAverage.h>
#include <Button.h>
#include <Config.h>
#include <Pins.h>

class Screen {
public:
    Screen() {}

    void init();

    void clearAll();
    void clear();

    void updateBatteryMeter();
    void showMainScreen();

private:
    TouchScreen ts = TouchScreen(SCREEN_XP, SCREEN_YP, SCREEN_XM, SCREEN_YM, TS_RESISTANCE);

    MovingAverage batteryAverage = MovingAverage(50);

    EngineStartButton engineStartButton;
    TextButton debugButton;
    TextButton settingsButton;
};

#endif // SCREEN_H
