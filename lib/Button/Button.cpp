#include "Button.h"

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_ILI9341_8bit *tft) : x(x), y(y), w(w), h(h), tft(tft) {}

void Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = tx > x && tx < x + w && ty > y && ty < y + h;
    highlighted = inButton && touchDown;
    return inButton && !touchDown;
}
