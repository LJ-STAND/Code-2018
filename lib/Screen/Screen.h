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
#include <Point.h>
#include <BluetoothData.h>

// Enum for the type of screens
enum ScreenType: uint8_t {
    mainScreenType, // Main screen
    debugScreenType, // Debug
    settingScreenType, // Settings
    imuDebugScreenType, // IMU debug
    motorsDebugScreenType, // Motors debug
    ballDebugScreenType, // Ball debug
    lightSensorsDebugScreenType, // Light sensors debug
    cameraDebugScreenType, // Camera debug
    ledDebugScreenType, // LED debug
    terminalDebugScreenType // Terminal
};

// Touchscreen controller. Is a subclass of print to enable printing to the terminal
class Screen : public Print {
public:
    Screen() {}

    void init();

    void clearAll();
    void clear();
    void changeScreen(ScreenType newType);
    void redrawScreen();
    
    void setOtherDebugData(bool otherData);
    void checkTouch();
    void updateBatteryMeter();
    void setBluetoothConnected(bool connected);
    void drawBluetoothIcon(bool connected);
    void update();

    void displayMessage(char *message, bool clearable = false);
    void clearMessage();

    size_t write(uint8_t c);

    DebugSettings settings; // Settings
    RGBType rgbType = RGBType::ballRGBType; // RGB data type

    // Debug Variables

    int heading = 0;
    int leftRPM = 0;
    int rightRPM = 0;
    int backLeftRPM = 0;
    int backRightRPM = 0;
    BallData ballData = BallData();
    LineData lineData = LineData();
    int yellowAngle = 0, yellowDistance = 0, blueAngle = 0, blueDistance = 0;
    uint16_t lsFirst = 0, lsSecond = 0, lsThird = 0, lsFourth = 0;
    long lsData = 0;
    Point robotPosition = Point();
    BluetoothData otherRobotData = BluetoothData();
    
private:
    TouchScreen ts = TouchScreen(SCREEN_XP, SCREEN_YP, SCREEN_XM, SCREEN_YM, TS_RESISTANCE); // Resistive touch controller object
    TSPoint lastTouchPoint; // Touch point
    bool lastIsTouching; // If the screen was previously touched

    bool displayingMessage = false; // Showing a popup message
    bool messageClearable = false; // If the popup message is clearable, for example by pressing the home button

    bool bluetoothConnected = false; // Bluetooth connection for the icon

    bool otherDebugData = false; // Showing the other robot's data on the debug screen

    MovingAverage batteryAverage = MovingAverage(50); // Average for smoothing the battery voltage

    ScreenType screenType = ScreenType::mainScreenType; // Type of screen being shown
    ScreenType lastScreenType = ScreenType::mainScreenType; // Last shown screen for back button

    // UI Elements //

    HomeButton homeButton;
    BackButton backButton;

    // Main Screen

    EngineStartButton engineStartButton;

    TextButton debugButton;
    TextButton settingsButton;
    TextButton IMUResetButton;
    TextButton lightSensorsResetButton;

    TextButton resetAllButton;

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

    Switch otherRobotDataSwitch;

    TextButton otherIMUDebugButton;
    TextButton otherBallDebugButton;

    IndicatorLabel otherBallIsOutLabel;
    IndicatorLabel otherIsOnFieldLabel;

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
    LightSensorView lightSensorView;

    // Camera Debug Screen

    GoalView goalView;
    RobotPositionView robotPositionView;

    // LED Debug Screen

    Label ballRGBLabel;
    CheckBox ballRGBCheckBox;

    Label lineRGBLabel;
    CheckBox lineRGBCheckBox;

    Label rainbowRGBLabel;
    CheckBox rainbowRGBCheckBox;

    Label goalRGBLabel;
    CheckBox goalRGBCheckBox;

    // Terminal Debug Screen

    Terminal terminal;
};

#endif // SCREEN_H
