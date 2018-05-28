#include "Screen.h"

void Screen::init() {
    settings.readEEPROM();

    TFT.begin();

    TFT.setRotation(SCREEN_ROTATION);
    TFT.fillScreen(BACKGROUND_COLOR);

    TFT.setTextSize(TITLE_FONT_SIZE);
    TFT.setTextColor(FOREGROUND_COLOR);

    TFT.setMaxCursor(TFT.width(), TFT.height());

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
    backButton = BackButton(BACK_BUTTON_X, BACK_BUTTON_Y);

    // Main Screen

    engineStartButton = EngineStartButton(TFT.width() / 2, LINE_Y + (TFT.height() - LINE_Y) / 2);
    debugButton = TextButton(TFT.width() - MAIN_SCREEN_BUTTON_INSET, LINE_Y + MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Debug", 1);
    settingsButton = TextButton(TFT.width() - MAIN_SCREEN_BUTTON_INSET, TFT.height() - MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Settings", 1);
    IMUResetButton = TextButton(MAIN_SCREEN_BUTTON_INSET, TFT.height() - MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Reset IMU", 1);
    lightSensorsResetButton = TextButton(MAIN_SCREEN_BUTTON_INSET, LINE_Y + MAIN_SCREEN_BUTTON_INSET, MAIN_SCREEN_BUTTON_RADIUS, "Reset LS", 1);

    // Debug Screen

    imuDebugButton = TextButton(120, LINE_Y + 40, 30, "IMU", 1);
    motorsDebugButton = TextButton(200, LINE_Y + 40, 30, "Motors", 1);
    ballDebugButton = TextButton(80, LINE_Y + (TFT.height() - LINE_Y) / 2, 30, "Ball", 1);
    lightSensorsDebugButton = TextButton(160, LINE_Y + (TFT.height() - LINE_Y) / 2, 30, "Light", 1);
    cameraDebugButton = TextButton(240, LINE_Y + (TFT.height() - LINE_Y) / 2, 30, "Camera", 1);
    ledsDebugButton = TextButton(120, TFT.height() - 40, 30, "LEDs", 1);
    terminalDebugButton = TextButton(200, TFT.height() - 40, 30, "Terminal", 1);

    // Settings Screen

    playModeSwitchingLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Play mode switching", 2);
    playModeSwitchingCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET);

    defaultPlayModeLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Default play mode", 2);
    defaultPlayModeSwitch = Switch(TFT.width() - SWITCH_WIDTH - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE, FOREGROUND_COLOR, "A", FOREGROUND_COLOR, "D");

    goalIsYellowLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 3 + CHECK_BOX_OUTER_SIZE * 2 + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Goal color", 2);
    goalIsYellowSwitch = Switch(TFT.width() - SWITCH_WIDTH - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 3 + CHECK_BOX_OUTER_SIZE * 2, YELLOW, "", BLUE, "");

    gameModeLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 4 + CHECK_BOX_OUTER_SIZE * 3 + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Game mode", 2);
    gameModeCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 4 + CHECK_BOX_OUTER_SIZE * 3);

    // IMU Debug Screen

    headingDial = Dial(TFT.width() / 2, LINE_Y + (TFT.height() - LINE_Y) / 2, 80, 360);

    // Motors Debug Screen

    leftRPMDial = Dial(TFT.width() / 2 - 50, LINE_Y + 50, 40, 1600);
    rightRPMDial = Dial(TFT.width() / 2 + 50, LINE_Y + 50, 40, 1600);
    backLeftRPMDial = Dial(TFT.width() / 2 - 50, TFT.height() - 50, 40, 1600);
    backRightRPMDial = Dial(TFT.width() / 2 + 50, TFT.height() - 50, 40, 1600);

    // Ball Debug Screen

    ballView = BallView(5, LINE_Y + 5, TFT.width() - 10, TFT.height() - LINE_Y - 10);

    // Light Sensor Debug Screen

    lineView = LineView(10, LINE_Y + 5, (TFT.width() - 20) / 3, TFT.height() - LINE_Y - 10);
    lightSensorView = LightSensorView(TFT.width() / 3, LINE_Y + 5, 2 * (TFT.width() - 20) / 3, TFT.height() - LINE_Y - 10);

    // Camera Debug Screen

    goalView = GoalView(5, LINE_Y + 5, (TFT.width() - 10) / 2, TFT.height() - LINE_Y - 10);
    robotPositionView = RobotPositionView(TFT.width() / 2, LINE_Y + 5, (TFT.width() - 10) / 2, TFT.height() - LINE_Y - 10);

    // LED Debug Screen

    ballRGBLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Ball", 2);
    ballRGBCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET);

    lineRGBLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Line", 2);
    lineRGBCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 2 + CHECK_BOX_OUTER_SIZE);

    rainbowRGBLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 3 + CHECK_BOX_OUTER_SIZE * 2 + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Rainbow", 2);
    rainbowRGBCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 3 + CHECK_BOX_OUTER_SIZE * 2);

    goalRGBLabel = Label(SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 4 + CHECK_BOX_OUTER_SIZE * 3 + SETTINGS_SCREEN_LABEL_Y_OFFSET, 200, 20, "Goal", 2);
    goalRGBCheckBox = CheckBox(TFT.width() - CHECK_BOX_OUTER_SIZE - SETTINGS_SCREEN_INSET, LINE_Y + SETTINGS_SCREEN_INSET * 4 + CHECK_BOX_OUTER_SIZE * 3);

    // Terminal Debug Screen

    terminal = Terminal(5, LINE_Y + 5, TFT.width() - 10, TFT.height() - LINE_Y - 10, 1, FOREGROUND_COLOR);
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

        if (!displayingMessage) {
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
                if (imuDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::imuDebugScreenType);
                }

                if (motorsDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::motorsDebugScreenType);
                }

                if (ballDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::ballDebugScreenType);
                }

                if (lightSensorsDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::lightSensorsDebugScreenType);
                }

                if (cameraDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::cameraDebugScreenType);
                }

                if (ledsDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::ledDebugScreenType);
                }

                if (terminalDebugButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    changeScreen(ScreenType::terminalDebugScreenType);
                }

                break;

            case ScreenType::settingScreenType:
                if (playModeSwitchingCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    settings.playModeSwitching = !settings.playModeSwitching;
                    settings.writeEEPROM();
                }

                if (defaultPlayModeSwitch.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    settings.defaultPlayModeIsAttack = !settings.defaultPlayModeIsAttack;
                    settings.writeEEPROM();
                }

                if (goalIsYellowSwitch.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    settings.goalIsYellow = !settings.goalIsYellow;
                    settings.writeEEPROM();
                }

                if (gameModeCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    settings.gameMode = !settings.gameMode;
                    settings.writeEEPROM();
                }

                break;

            case ScreenType::ledDebugScreenType:
                if (ballRGBCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    rgbType = RGBType::ballRGBType;
                }

                if (lineRGBCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    rgbType = RGBType::lineRGBType;
                }

                if (rainbowRGBCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    rgbType = RGBType::rainbowRGBType;
                }

                if (goalRGBCheckBox.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
                    rgbType = RGBType::goalRGBType;
                }

                break;
            }
        }

        if (homeButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
            changeScreen(ScreenType::mainScreenType);
        }

        if (backButton.isTouched(currentTouchPoint.x, currentTouchPoint.y, isTouching)) {
            changeScreen(lastScreenType);
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
            imuDebugButton.checkDraw();
            motorsDebugButton.checkDraw();
            ballDebugButton.checkDraw();
            lightSensorsDebugButton.checkDraw();
            cameraDebugButton.checkDraw();
            ledsDebugButton.checkDraw();
            terminalDebugButton.checkDraw();

            break;

        case ScreenType::settingScreenType:
            playModeSwitchingCheckBox.setEnabled(settings.playModeSwitching);
            defaultPlayModeSwitch.setEnabled(settings.defaultPlayModeIsAttack);
            goalIsYellowSwitch.setEnabled(settings.goalIsYellow);
            gameModeCheckBox.setEnabled(settings.gameMode);

            playModeSwitchingLabel.checkDraw();
            playModeSwitchingCheckBox.checkDraw();

            defaultPlayModeLabel.checkDraw();
            defaultPlayModeSwitch.checkDraw();

            goalIsYellowLabel.checkDraw();
            goalIsYellowSwitch.checkDraw();

            gameModeLabel.checkDraw();
            gameModeCheckBox.checkDraw();

            break;

        case ScreenType::imuDebugScreenType:
            headingDial.setValue(heading);
            headingDial.checkDraw();
            
            break;

        case ScreenType::motorsDebugScreenType:
            leftRPMDial.setValue(leftRPM);
            leftRPMDial.checkDraw();

            rightRPMDial.setValue(rightRPM);
            rightRPMDial.checkDraw();

            backLeftRPMDial.setValue(backLeftRPM);
            backLeftRPMDial.checkDraw();

            backRightRPMDial.setValue(backRightRPM);
            backRightRPMDial.checkDraw();

            break;

        case ScreenType::ballDebugScreenType:
            ballView.setBallData(ballData);
            ballView.checkDraw();

            break;

        case ScreenType::lightSensorsDebugScreenType:
            lineView.setLineData(lineData);
            lineView.checkDraw();

            lightSensorView.setLightSensorData(lsFirst | (lsSecond << 10) | (lsThird << 20) | (lsFourth << 30));
            lightSensorView.checkDraw();

            break;

        case ScreenType::cameraDebugScreenType:
            goalView.setGoalData(yellowAngle, yellowDistance, blueAngle, blueDistance);
            goalView.checkDraw();

            robotPositionView.setRobotPositionData(robotPositionX, robotPositionY);
            robotPositionView.checkDraw();

            break;

        case ScreenType::ledDebugScreenType:
            ballRGBCheckBox.setEnabled(rgbType == RGBType::ballRGBType);
            lineRGBCheckBox.setEnabled(rgbType == RGBType::lineRGBType);
            rainbowRGBCheckBox.setEnabled(rgbType == RGBType::rainbowRGBType);
            goalRGBCheckBox.setEnabled(rgbType == RGBType::goalRGBType);

            ballRGBLabel.checkDraw();
            ballRGBCheckBox.checkDraw();

            lineRGBLabel.checkDraw();
            lineRGBCheckBox.checkDraw();

            rainbowRGBLabel.checkDraw();
            rainbowRGBCheckBox.checkDraw();
            
            goalRGBLabel.checkDraw();
            goalRGBCheckBox.checkDraw();

            break;

        case ScreenType::terminalDebugScreenType:
            terminal.drawFromBuffer();

            break;

        default:
            break;
        }
    }

    if (!displayingMessage || messageClearable) {
        homeButton.checkDraw();
        backButton.checkDraw();

        checkTouch();
    }

    updateBatteryMeter();
}

void Screen::redrawScreen() {
    clear();
    displayingMessage = false;

    switch (screenType) {
    case ScreenType::mainScreenType:
        engineStartButton.setNeedsDraw();
        debugButton.setNeedsDraw();
        settingsButton.setNeedsDraw();
        IMUResetButton.setNeedsDraw();
        lightSensorsResetButton.setNeedsDraw();

        break;

    case ScreenType::debugScreenType:
        if (settings.gameMode) {
            displayMessage("Game mode - no debug", true);
        } else {
            imuDebugButton.setNeedsDraw();
            motorsDebugButton.setNeedsDraw();
            ballDebugButton.setNeedsDraw();
            lightSensorsDebugButton.setNeedsDraw();
            cameraDebugButton.setNeedsDraw();
            ledsDebugButton.setNeedsDraw();
            terminalDebugButton.setNeedsDraw();
        }
        
        break;

    case ScreenType::settingScreenType:
        playModeSwitchingLabel.setNeedsDraw();
        playModeSwitchingCheckBox.setNeedsDraw();

        defaultPlayModeLabel.setNeedsDraw();
        defaultPlayModeSwitch.setNeedsDraw();

        goalIsYellowLabel.setNeedsDraw();
        goalIsYellowSwitch.setNeedsDraw();

        gameModeLabel.setNeedsDraw();
        gameModeCheckBox.setNeedsDraw();

        break;

    case ScreenType::imuDebugScreenType:
        headingDial.setNeedsDraw();

        break;

    case ScreenType::motorsDebugScreenType:
        leftRPMDial.setNeedsDraw();
        rightRPMDial.setNeedsDraw();
        backLeftRPMDial.setNeedsDraw();
        backRightRPMDial.setNeedsDraw();

        break;

    case ScreenType::ballDebugScreenType:
        ballView.setNeedsDraw();

        break;

    case ScreenType::lightSensorsDebugScreenType:
        lineView.setNeedsDraw();
        lightSensorView.setNeedsDraw();

        break;

    case ScreenType::cameraDebugScreenType:
        goalView.setNeedsDraw();

        break;

    case ScreenType::ledDebugScreenType:
        ballRGBLabel.setNeedsDraw();
        ballRGBCheckBox.setNeedsDraw();

        lineRGBLabel.setNeedsDraw();
        lineRGBCheckBox.setNeedsDraw();

        rainbowRGBLabel.setNeedsDraw();
        rainbowRGBCheckBox.setNeedsDraw();

        goalRGBLabel.setNeedsDraw();
        goalRGBCheckBox.setNeedsDraw();

        break;

    case ScreenType::terminalDebugScreenType:
        terminal.setNeedsDraw();

        break;

    default:
        break;
    }
}

void Screen::changeScreen(ScreenType newType) {
    if (screenType != newType) {
        lastScreenType = screenType;
        screenType = newType;

        redrawScreen();
    }
}

void Screen::updateBatteryMeter() {
    batteryAverage.update(analogRead(BATTERY_VOLTAGE));

    double batteryLevel = (double)(fmin(fmax(batteryAverage.average() * BATTERY_VOLTAGE_MULTIPILER, MIN_BATTERY_VOLTAGE), MAX_BATTERY_VOLTAGE) - MIN_BATTERY_VOLTAGE) / (double)(MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);
    uint16_t batteryMeterWidth = (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel;

    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET, BATTERY_METER_Y + BATTERY_METER_INSET, batteryMeterWidth, BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, batteryLevel < 0.2 ? RED : GREEN);
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET + batteryMeterWidth, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) - batteryMeterWidth, BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, BACKGROUND_COLOR);
}

void Screen::displayMessage(char *message, bool clearable) {
    messageClearable = clearable;
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
        redrawScreen();
    }
}

size_t Screen::write(uint8_t c) {
    if (screenType == ScreenType::terminalDebugScreenType) {
        terminal.write(c);
    }

    return 1;
}

void Screen::setBluetoothConnected(bool connected) {
    if (bluetoothConnected != connected) {
        bluetoothConnected = connected;
        drawBluetoothIcon(connected);
    }
}

void Screen::drawBluetoothIcon(bool connected) {
    if (connected) {
        TFT.drawLine(BLUETOOTH_ICON_X, BLUETOOTH_ICON_Y + BLUETOOTH_ICON_HEIGHT / 4, BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH, BLUETOOTH_ICON_Y + 3 * BLUETOOTH_ICON_HEIGHT / 4, FOREGROUND_COLOR);
        TFT.drawLine(BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH, BLUETOOTH_ICON_Y + 3 * BLUETOOTH_ICON_HEIGHT / 4, BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH / 2, BLUETOOTH_ICON_Y + BLUETOOTH_ICON_HEIGHT, FOREGROUND_COLOR);
        TFT.drawLine(BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH / 2, BLUETOOTH_ICON_Y + BLUETOOTH_ICON_HEIGHT, BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH / 2, BLUETOOTH_ICON_Y, FOREGROUND_COLOR);
        TFT.drawLine(BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH / 2, BLUETOOTH_ICON_Y, BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH, BLUETOOTH_ICON_Y + BLUETOOTH_ICON_HEIGHT / 4, FOREGROUND_COLOR);
        TFT.drawLine(BLUETOOTH_ICON_X + BLUETOOTH_ICON_WIDTH, BLUETOOTH_ICON_Y + BLUETOOTH_ICON_HEIGHT / 4, BLUETOOTH_ICON_X, BLUETOOTH_ICON_Y + 3 * BLUETOOTH_ICON_HEIGHT / 4, FOREGROUND_COLOR);
    } else {
        TFT.fillRect(BLUETOOTH_ICON_X, BLUETOOTH_ICON_Y, BLUETOOTH_ICON_WIDTH + 2, BLUETOOTH_ICON_HEIGHT + 2, BACKGROUND_COLOR);
    }
}
