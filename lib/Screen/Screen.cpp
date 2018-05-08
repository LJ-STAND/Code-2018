#include "Screen.h"

void Screen::init() {
    TFT.begin();

    TFT.setRotation(SCREEN_ROTATION);
    TFT.fillScreen(BLACK);


    TFT.setTextSize(TITLE_FONT_SIZE);

    uint16_t width, height;
    int16_t tempX, tempY;

    TFT.getTextBounds("LJ STAND", 0, 0, &tempX, &tempY, &width, &height);

    TFT.setCursor(TFT.width() / 2 - width / 2, TITLE_Y);
    TFT.print("LJ STAND");

    TFT.drawRoundRect(TFT.width() - BATTERY_METER_RIGHT_X, BATTERY_METER_Y, BATTERY_METER_WIDTH, BATTERY_METER_HEIGHT, BATTERY_METER_ROUNDED_RADIUS, WHITE);
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH - 1, BATTERY_METER_END_HEIGHT, WHITE);
    TFT.fillRoundRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH, BATTERY_METER_END_HEIGHT, BATTERY_METER_END_ROUNDED_RADIUS, WHITE);

    TFT.drawLine(0, LINE_Y, TFT.width(), LINE_Y, WHITE);

    engineStartButton = EngineStartButton(TFT.width() / 2, LINE_Y + (TFT.height() - LINE_Y) / 2);
    debugButton = TextButton(TFT.width() - 50, LINE_Y + 50, 30, "Debug", 1);
    settingsButton = TextButton(TFT.width() - 50, TFT.height() - 50, 30, "Settings", 1);
    headingResetButton = TextButton(50, LINE_Y + 50, 30, "Reset Heading", 1);
    IMUCalibrateButton = TextButton(50, TFT.height() - 50, 30, "Calibrate IMU", 1);
    homeButton = HomeButton(15, 12);

    testCheckBox = CheckBox(30, LINE_Y + 30);
}

void Screen::clearAll() {
    TFT.fillScreen(BLACK);
}

void Screen::clear() {
    TFT.fillRect(0, LINE_Y + 1, TFT.width(), TFT.height() - LINE_Y, BLACK);
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

            if (headingResetButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                settings.headingNeedsResetting = true;
            }

            if (IMUCalibrateButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                settings.IMUNeedsCalibrating = true;
                settings.engineStarted = false;
                displayMessage("Calibrating IMU...");
            }

            break;

        case ScreenType::debugScreenType:
            break;

        case ScreenType::settingScreenType:
            if (testCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                testCheckBox.setEnabled(!testCheckBox.getEnabled());
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
            IMUCalibrateButton.checkDraw();
            headingResetButton.checkDraw();

            break;

        case ScreenType::debugScreenType:
            if (updateTextTimer.timeHasPassed()) {
                TFT.fillRect(10, LINE_Y + 10, 40, 20, BLACK);
                TFT.setCursor(10, LINE_Y + 10);
                TFT.setTextSize(2);
                TFT.print(heading);
            }

            break;

        case ScreenType::settingScreenType:
            testCheckBox.checkDraw();

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
        IMUCalibrateButton.setNeedsDraw();
        headingResetButton.setNeedsDraw();

        break;

    case ScreenType::settingScreenType:
        testCheckBox.setNeedsDraw();
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
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET + (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * (1.0 - batteryLevel), BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, BLACK);
}

void Screen::displayMessage(char *message) {
    displayingMessage = true;

    TFT.setTextSize(MESSAGE_BOX_FONT_SIZE);

    uint16_t width, height;
    int16_t tempX, tempY;

    TFT.getTextBounds(message, 0, 0, &tempX, &tempY, &width, &height);

    TFT.setCursor(TFT.width() / 2 - width / 2, TFT.height() / 2 - height / 2);

    TFT.fillRect(TFT.width() / 2 - (width + MESSAGE_BOX_PADDING) / 2, TFT.height() / 2 - (height + MESSAGE_BOX_PADDING) / 2, width + MESSAGE_BOX_PADDING, height + MESSAGE_BOX_PADDING, BLACK);
    TFT.drawRect(TFT.width() / 2 - (width + MESSAGE_BOX_PADDING) / 2, TFT.height() / 2 - (height + MESSAGE_BOX_PADDING) / 2, width + MESSAGE_BOX_PADDING, height + MESSAGE_BOX_PADDING, WHITE);

    TFT.print(message);
}

void Screen::clearMessage() {
    if (displayingMessage) {
        displayingMessage = false;

        redrawScreen();
    }
}
