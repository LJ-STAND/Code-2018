#include "Button.h"

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_ILI9341_8bit &tft) : x(x), y(y), w(w), h(h), tft(&tft) {}

bool Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = tx > x && tx < x + w && ty > y && ty < y + h;
    highlighted = inButton && touchDown;
    return inButton && !touchDown;
}

EngineStartButton::EngineStartButton(uint16_t x, uint16_t y, Adafruit_ILI9341_8bit &tft) : x(x), y(y), tft(&tft) {}

void EngineStartButton::draw() {
    uint16_t widthEngine, heightEngine, widthStart, heightStart;
    int16_t x, y;

    tft->fillCircle(x, y, ENGINE_START_BUTTON_OUTER_RADIUS, tft->color565(200, 200, 200));
    tft->fillCircle(x, y, ENGINE_START_BUTTON_MIDDLE_RADIUS, tft->color565(150, 150, 150));
    tft->fillCircle(x, y, ENGINE_START_BUTTON_INNER_RADIUS, tft->color565(30, 30, 30));

    tft->setTextSize(ENGINE_START_BUTTON_ENGINE_TEXT_SIZE);
    tft->getTextBounds("ENGINE", 0, 0, &x, &y, &widthEngine, &heightEngine);
    tft->setCursor(x - widthEngine / 2, y + ENGINE_START_BUTTON_ENGINE_TEXT_Y);
    tft->print("ENGINE");
    tft->setTextSize(ENGINE_START_BUTTON_START_TEXT_SIZE);
    tft->getTextBounds("START", 0, 0, &x, &y, &widthStart, &heightStart);
    tft->setCursor(x - widthStart / 2, y + ENGINE_START_BUTTON_START_TEXT_Y);
    tft->print("START");

    tft->fillRoundRect(x - ENGINE_START_BUTTON_INDICATOR_WIDTH / 2, y + ENGINE_START_BUTTON_INDICATOR_Y, ENGINE_START_BUTTON_INDICATOR_WIDTH, ENGINE_START_BUTTON_INDICATOR_HEIGHT, ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS, enabled ? GREEN : RED);
}

TextButton::TextButton(uint16_t x, uint16_t y, uint16_t r, String str, uint8_t textSize, Adafruit_ILI9341_8bit &screen) : x(x), y(y), r(r), str(str), textSize(textSize), tft(&screen) {
    int16_t x, y;

    tft->setTextSize(textSize);
    tft->getTextBounds(str, 0, 0, &x, &y, &textWidth, &textHeight);
}

void TextButton::draw() {
    tft->drawCircle(x, y, r, WHITE);
    tft->setCursor(x - textWidth / 2, y - textHeight / 2);
    tft->setTextSize(textSize);
    tft->print(str);
}
