#include "Screen.h"

void Screen::init() {
    tft.begin();

    tft.setRotation(SCREEN_ROTATION);
    tft.fillScreen(BLACK);

    tft.setCursor(TITLE_X, TITLE_Y);
    tft.setTextSize(TITLE_FONT_SIZE);
    tft.print("LJ STAND");

    tft.drawRoundRect(tft.width() - BATTERY_METER_RIGHT_X, BATTERY_METER_Y, BATTERY_METER_WIDTH, BATTERY_METER_HEIGHT, BATTERY_METER_ROUNDED_RADIUS, WHITE);
    tft.fillRect(tft.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH - 1, BATTERY_METER_END_HEIGHT, WHITE);
    tft.fillRoundRect(tft.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_WIDTH, BATTERY_METER_Y + (BATTERY_METER_HEIGHT - BATTERY_METER_END_HEIGHT) / 2, BATTERY_METER_END_WIDTH, BATTERY_METER_END_HEIGHT, BATTERY_METER_END_ROUNDED_RADIUS, WHITE);

    tft.drawLine(0, LINE_Y, tft.width(), LINE_Y, WHITE);

    engineStartButton = EngineStartButton(tft.width() / 2, LINE_Y + (tft.height() - LINE_Y) / 2, tft);
    debugButton = TextButton(tft.width() - 70, tft.height() / 2 - 40, 50, "Debug", 1, tft);
}

void Screen::clearAll() {
    tft.fillScreen(BLACK);
}

void Screen::clear() {
    tft.fillRect(0, LINE_Y + 1, tft.width(), tft.height() - LINE_Y, BLACK);
}

void Screen::showMainScreen() {

}

void Screen::updateBatteryMeter() {
    batteryAverage.update(analogRead(BATTERY_VOLTAGE));

    double batteryLevel = (double)(fmin(fmax(batteryAverage.average() * BATTERY_VOLTAGE_MULTIPILER, MIN_BATTERY_VOLTAGE), MAX_BATTERY_VOLTAGE) - MIN_BATTERY_VOLTAGE) / (double)(MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);

    tft.fillRect(tft.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET, BATTERY_METER_Y + BATTERY_METER_INSET, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, batteryLevel < 0.2 ? RED : GREEN);
    tft.fillRect(tft.width() - BATTERY_METER_RIGHT_X + BATTERY_METER_INSET + (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * batteryLevel, 9, (BATTERY_METER_WIDTH - 2 * BATTERY_METER_INSET) * (1.0 - batteryLevel), BATTERY_METER_HEIGHT - 2 * BATTERY_METER_INSET, BLACK);
}
