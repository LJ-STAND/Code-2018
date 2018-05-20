#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <MovingAverage.h>
#include <ScreenUI.h>
#include <Config.h>
#include <Pins.h>
#include <DebugSettings.h>
#include <Timer.h>
#include <BallData.h>
#include <LED.h>

enum ScreenType: uint8_t {
    mainScreenType,
    debugScreenType,
    settingScreenType,
    imuDebugScreenType,
    motorsDebugScreenType,
    ballDebugScreenType,
    lightSensorsDebugScreenType,
    cameraDebugScreenType,
    ledDebugScreenType,
    terminalDebugScreenType
};

class Screen : public Print {
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

    void displayMessage(char *message, bool clearable = false);
    void clearMessage();

    size_t write(uint8_t c);

    DebugSettings settings;
    RGBType rgbType = RGBType::ballRGBType;

    // Debug Variables

    int heading;
    int leftRPM;
    int rightRPM;
    int backLeftRPM;
    int backRightRPM;
    BallData ballData;
    LineData lineData;

private:
    TouchScreen ts = TouchScreen(SCREEN_XP, SCREEN_YP, SCREEN_XM, SCREEN_YM, TS_RESISTANCE);
    TSPoint lastTouchPoint;
    bool lastIsTouching;

    bool displayingMessage = false;
    bool messageClearable = false;

    MovingAverage batteryAverage = MovingAverage(50);

    ScreenType screenType = ScreenType::mainScreenType;
    ScreenType lastScreenType = ScreenType::mainScreenType;

    HomeButton homeButton;
    BackButton backButton;

    // Main Screen

    EngineStartButton engineStartButton;

    TextButton debugButton;
    TextButton settingsButton;
    TextButton IMUResetButton;
    TextButton lightSensorsResetButton;

    // Settings Screen

    Label playModeSwitchingLabel;
    CheckBox playModeSwitchingCheckBox;

    Label defaultPlayModeLabel;
    Switch defaultPlayModeSwitch;

    Label goalIsYellowLabel;
    Switch goalIsYellowSwitch;

    Label gameModeLabel;
    CheckBox gameModeCheckBox;

    // Debug Screen

    TextButton imuDebugButton;
    TextButton motorsDebugButton;
    TextButton ballDebugButton;
    TextButton lightSensorsDebugButton;
    TextButton cameraDebugButton;
    TextButton ledsDebugButton;
    TextButton terminalDebugButton;

    // IMU Debug Screen

    Dial headingDial;

    // Motor Debug Screen

    Dial leftRPMDial;
    Dial rightRPMDial;
    Dial backLeftRPMDial;
    Dial backRightRPMDial;

    // Ball Debug Screen

    BallView ballView;

    // Light Sensor Debug Screen

    LineView lineView;

    // Camera Debug Screen

    // LED Debug Screen

    Label ballRGBLabel;
    CheckBox ballRGBCheckBox;

    Label lineRGBLabel;
    CheckBox lineRGBCheckBox;

    Label rainbowRGBLabel;
    CheckBox rainbowRGBCheckBox;

    Label customRGBLabel;
    CheckBox customRGBCheckBox;

    // Terminal Debug Screen

    Terminal terminal;

    Timer updateTextTimer = Timer(SCREEN_UPDATE_TEXT_TIME);
};

#endif // SCREEN_H
