#include "Button.h"

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_ILI9341_8bit *tft) : x(x), y(y), w(w), h(h), tft(tft) {}

void Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = tx > x && tx < x + w && ty > y && ty < y + h;
    highlighted = inButton && touchDown;
    return inButton && !touchDown;
}

void EngineStartButton::draw() {
    uint16_t width1, height1, width2, height2;
    int16_t x, y;

    tft.fillCircle(tft.width() / 2, tft.height() / 2, 78, tft.color565(200, 200, 200));
    tft.fillCircle(tft.width() / 2, tft.height() / 2, 75, tft.color565(150, 150, 150););
    tft.fillCircle(tft.width() / 2, tft.height() / 2, 70, tft.color565(30, 30, 30));
    tft.fillRoundRect(tft.width() / 2 - 20, tft.height() / 2 - 40, 40, 10, 3, RED);
    tft.setTextSize(2);
    tft.getTextBounds("ENGINE", 0, 0, &x, &y, &width1, &height1);
    tft.setCursor(tft.width() / 2 - width1 / 2, tft.height() / 2 - 10);
    tft.print("ENGINE");
    tft.setTextSize(3);
    tft.getTextBounds("START", 0, 0, &x, &y, &width2, &height2);
    tft.setCursor(tft.width() / 2 - width2 / 2, tft.height() / 2 + 10);
    tft.print("START");

    tft.fillRoundRect(tft.width() / 2 - 20, tft.height() / 2 - 40, 40, 10, 3, enabled ? GREEN : RED);
}
