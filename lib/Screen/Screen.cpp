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
    debugButton = TextButton(TFT.width() - 35, LINE_Y + (TFT.height() - LINE_Y) / 2 - 40, 30, "Debug", 1);
    settingsButton = TextButton(TFT.width() - 35, LINE_Y + (TFT.height() - LINE_Y) / 2 + 40, 30, "Settings", 1);

    showMainScreen();
}

void Screen::clearAll() {
    TFT.fillScreen(BLACK);
}

void Screen::clear() {
    TFT.fillRect(0, LINE_Y + 1, TFT.width(), TFT.height() - LINE_Y, BLACK);
}

void Screen::showMainScreen() {
    engineStartButton.draw();
    debugButton.draw();
    settingsButton.draw();
}

void Screen::updateBatteryMeter() {
    batteryAverage.update(analogRead(BATTERY_VOLTAGE));

    double batteryLevel = (double)(fmin(fmax(batteryAverage.average() * BATTERY_VOLTAGE_MULTIPILER, MIN_BATTERY_VOLTAGE), MAX_BATTERY_VOLTAGE) - MIN_BATTERY_VOLTAGE) / (double)(MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);

    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, batteryLevel < 0.2 ? RED : GREEN);
    TFT.fillRect(TFT.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET + (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * (1.0 - batteryLevel), BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, BLACK);
}
