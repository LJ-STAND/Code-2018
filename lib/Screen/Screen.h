#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <MovingAverage.h>
#include <Button.h>
#include <Config.h>
#include <Pins.h>
#include <DebugSettings.h>
#include <Timer.h>

enum ScreenType: uint8_t {
    mainScreenType,
    debugScreenType,
    settingScreenType
};

class Screen {
public:
    Screen() {}

    void init();

    void clearAll();
    void clear();
    void changeScreen(ScreenType newType);
    void redrawScreen();

    void checkTouch();
    void updateBatteryMeter();
    void update();

    void displayMessage(char *message);
    void clearMessage();

    DebugSettings settings;

    int heading;

private:
    TouchScreen ts = TouchScreen(SCREEN_XP, SCREEN_YP, SCREEN_XM, SCREEN_YM, TS_RESISTANCE);
    TSPoint lastTouchPoint;
    bool lastIsTouching;

    bool displayingMessage = false;

    MovingAverage batteryAverage = MovingAverage(50);

    ScreenType screenType = ScreenType::mainScreenType;

    EngineStartButton engineStartButton;
    TextButton debugButton;
    TextButton settingsButton;
    TextButton IMUCalibrateButton;
    TextButton headingResetButton;
    HomeButton homeButton;

    CheckBox testCheckBox;

    Timer updateTextTimer = Timer(SCREEN_UPDATE_TEXT_TIME);
};

#endif // SCREEN_H
