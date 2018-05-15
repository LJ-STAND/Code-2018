#include "Screen.h"

void Screen::init() {
    TFT.begin();

    TFT.setRotation(SCREEN_ROTATION);
    TFT.fillScreen(BACKGROUND_COLOR);

    TFT.setTextSize(TITLE_FONT_SIZE);
    TFT.setTextColor(FOREGROUND_COLOR);

    uint16_t width, height;
    int16_t tempX, tempY;

    TFT.getTextBounds("LJ STAND", 0, 0, &tempX, &tempY, &width, &height);

    TFT.setCursor(TFT.width() / 2 - width / 2, TITLE_Y);
    TFT.print("LJ STAND");

    TFT.drawRoundRect(TFT.width() - BATTERY_METER_RIGHT_X, BATTERY_METER_Y, BATTERY_METER_WIDTH, BATTERY_METER_HEIGHT, BATTERY_METER_ROUNDED_RADIUS, FOREGROUND_COLOR);
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH - 1, BATTERY_METER_END_HEIGHT, FOREGROUND_COLOR);
    TFT.fillRoundRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH, BATTERY_METER_END_HEIGHT, BATTERY_METER_END_ROUNDED_RADIUS, FOREGROUND_COLOR);

    TFT.drawLine(0, LINE_Y, TFT.width(), LINE_Y, FOREGROUND_COLOR);

    homeButton = HomeButton(HOME_BUTTON_X, HOME_BUTTON_Y);

    // Main Screen

    engineStartButton = EngineStartButton(TFT.width() / 2, LINE_Y + (TFT.height() - LINE_Y) / 2);
    debugButton = TextButton(TFT.width() - MAIN_SCREEN_BUTTON_INSET, LINE_Y + MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Debug", 1);
    settingsButton = TextButton(TFT.width() - MAIN_SCREEN_BUTTON_INSET, TFT.height() - MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Settings", 1);
    IMUResetButton = TextButton(MAIN_SCREEN_BUTTON_INSET, TFT.height() - MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Reset IMU", 1);
    lightSensorsResetButton = TextButton(MAIN_SCREEN_BUTTON_INSET, LINE_Y + MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Reset LS", 1);

    // Debug Screen

    headingLabel = Label(10, LINE_Y + 10, 150, 20, "Heading: 0", 2);
    headingDial = Dial(TFT.width() / 2, TFT.height() / 2, 50, 360, 0, 0);

    // Settings Screen

    playModeSwitchingLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Play mode switching", 2);
    playModeSwitchingCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET);

    defaultPlayModeLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Default play mode", 2);
    defaultPlayModeSwitch = Switch(TFT.width() - SWITCH_WIDTH - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE, WHITE, "A", WHITE, "D");
}

void Screen::clearAll() {
    TFT.fillScreen(BACKGROUND_COLOR);
}

void Screen::clear() {
    TFT.fillRect(0, LINE_Y + 1, TFT.width(), TFT.height() - LINE_Y, BACKGROUND_COLOR);
}

void Screen::checkTouch() {
    TSPoint p = ts.getPoint();

    bool isTouching = p.z > MIN_PRESSURE && p.z < MAX_PRESSURE && p.x > TS_MINX && p.y < TS_MAXX && p.y > TS_MINY && p.y < TS_MAXY;

    if (isTouching || lastIsTouching) {
        TSPoint touchPoint = p;
        touchPoint.x = map(p.y, TS_MAXY, TS_MINY, 0, TFT.width());
        touchPoint.y = map(p.x, TS_MINX, TS_MAXX, 0, TFT.height());

        TSPoint currentTouchPoint = isTouching ? touchPoint : lastTouchPoint;
        lastTouchPoint = touchPoint;

        switch (screenType) {
        case ScreenType::mainScreenType:
            if (engineStartButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                settings.engineStarted = !settings.engineStarted;
            }

            if (debugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                changeScreen(ScreenType::debugScreenType);
            }

            if (settingsButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                changeScreen(ScreenType::settingScreenType);
            }

            if (IMUResetButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                settings.IMUNeedsResetting = true;
                settings.engineStarted = false;
                displayMessage("Resetting IMU...");
            }

            if (lightSensorsResetButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                settings.lightSensorsNeedResetting = true;
                settings.engineStarted = false;
                displayMessage("Resetting LS...");
            }

            break;

        case ScreenType::debugScreenType:
            break;

        case ScreenType::settingScreenType:
            if (playModeSwitchingCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                playModeSwitchingCheckBox.setEnabled(!playModeSwitchingCheckBox.getEnabled());
            }

            if (defaultPlayModeSwitch.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                defaultPlayModeSwitch.setEnabled(!defaultPlayModeSwitch.getEnabled());
            }

            break;
        }

        if (homeButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
            changeScreen(ScreenType::mainScreenType);
        }
    }

    lastIsTouching = isTouching;
}

void Screen::update() {
    if (!displayingMessage) {
        switch (screenType) {
        case ScreenType::mainScreenType:
            engineStartButton.setEnabled(settings.engineStarted);
            engineStartButton.checkDraw();
            debugButton.checkDraw();
            settingsButton.checkDraw();
            IMUResetButton.checkDraw();
            lightSensorsResetButton.checkDraw();

            break;

        case ScreenType::debugScreenType:
            if (updateTextTimer.timeHasPassed()) {
                char headingString[50];
                sprintf(headingString, "Heading: %d", heading);

                headingLabel.setText(headingString);
                headingDial.setValue(heading);
            }

            headingLabel.checkDraw();
            headingDial.checkDraw();

            break;

        case ScreenType::settingScreenType:
            playModeSwitchingLabel.checkDraw();
            playModeSwitchingCheckBox.checkDraw();

            defaultPlayModeLabel.checkDraw();
            defaultPlayModeSwitch.checkDraw();

            break;
        }

        homeButton.checkDraw();

        checkTouch();
    }

    updateBatteryMeter();
}

void Screen::redrawScreen() {
    clear();

    switch (screenType) {
    case ScreenType::mainScreenType:
        engineStartButton.setNeedsDraw();
        debugButton.setNeedsDraw();
        settingsButton.setNeedsDraw();
        IMUResetButton.setNeedsDraw();
        lightSensorsResetButton.setNeedsDraw();
        headingDial.setNeedsDraw();

        break;

    case ScreenType::debugScreenType:
        headingLabel.setNeedsDraw();

        break;

    case ScreenType::settingScreenType:
        playModeSwitchingLabel.setNeedsDraw();
        playModeSwitchingCheckBox.setNeedsDraw();

        defaultPlayModeLabel.setNeedsDraw();
        defaultPlayModeSwitch.setNeedsDraw();

        break;
    }
}

void Screen::changeScreen(ScreenType newType) {
    if (screenType != newType) {
        screenType = newType;

        redrawScreen();
    }
}

void Screen::updateBatteryMeter() {
    batteryAverage.update(analogRead(BATTERY_VOLTAGE));

    double batteryLevel = (double)(fmin(fmax(batteryAverage.average() * BATTERY_VOLTAGE_MULTIPILER, MIN_BATTERY_VOLTAGE), MAX_BATTERY_VOLTAGE) - MIN_BATTERY_VOLTAGE) / (double)(MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);

    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, batteryLevel < 0.2 ? RED : GREEN);
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET + (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * (1.0 - batteryLevel), BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, BACKGROUND_COLOR);
}

void Screen::displayMessage(char *message) {
    displayingMessage = true;

    TFT.setTextSize(MESSAGE_BOX_FONT_SIZE);

    uint16_t width, height;
    int16_t tempX, tempY;

    TFT.getTextBounds(message, 0, 0, &tempX, &tempY, &width, &height);

    TFT.setCursor(TFT.width() / 2 - width / 2, TFT.height() / 2 - height / 2);

    TFT.fillRect(TFT.width() / 2 - (width + MESSAGE_BOX_PADDING) / 2, TFT.height() / 2 - (height + MESSAGE_BOX_PADDING) / 2, width + MESSAGE_BOX_PADDING, height + MESSAGE_BOX_PADDING, BACKGROUND_COLOR);
    TFT.drawRect(TFT.width() / 2 - (width + MESSAGE_BOX_PADDING) / 2, TFT.height() / 2 - (height + MESSAGE_BOX_PADDING) / 2, width + MESSAGE_BOX_PADDING, height + MESSAGE_BOX_PADDING, FOREGROUND_COLOR);

    TFT.print(message);
}

void Screen::clearMessage() {
    if (displayingMessage) {
        displayingMessage = false;

        redrawScreen();
    }
}
