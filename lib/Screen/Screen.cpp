#include "Screen.h"

void Screen::init() {
    tft.begin();

    tft.setRotation(3);
    tft.fillScreen(BLACK);

    tft.setCursor(5, 0);
    tft.setTextSize(3);
    tft.print("LJ STAND");

    tft.drawRoundRect(tft.width() - 45, 5, 40, 20, 4, WHITE);
    tft.fillRect(tft.width() - 5, 10, 2, 10, WHITE);
    tft.fillRoundRect(tft.width() - 5, 10, 3, 10, 2, WHITE);

    tft.drawLine(0, 30, tft.width(), 30, WHITE);
}

void Screen::updateBatteryMeter() {
    batteryAverage.update(analogRead(BATTERY_VOLTAGE));

    double batteryLevel = (double)(fmin(fmax(batteryAverage.average() * BATT_MULTIPILER, MIN_BATT_VOLTAGE), MAX_BATT_VOLTAGE) - MIN_BATT_VOLTAGE) / (double)(MAX_BATT_VOLTAGE - MIN_BATT_VOLTAGE);

    tft.fillRect(tft.width() - 41, 9, 32.0 * batteryLevel, 12, batteryLevel < 0.2 ? RED : GREEN);
    tft.fillRect(tft.width() - 9 - (32 * (1.0 - batteryLevel)), 9, 32.0 * (1.0 - batteryLevel), 12, BLACK);
}
