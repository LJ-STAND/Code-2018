#include "Button.h"

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : x(x), y(y), w(w), h(h) {}

CircleButton::CircleButton(uint16_t cx, uint16_t cy, uint16_t r) : Button(cx - r, cy - r, r * 2, r * 2), cx(cx), cy(cy), r(r) {}

bool Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = tx > x && tx < x + w && ty > y && ty < y + h;
    highlighted = inButton && touchDown;
    return inButton && !touchDown;
}

bool CircleButton::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = distanceBetween(cx, cy, tx, ty) < r;
    Button::highlighted = inButton && touchDown;
    return inButton && !touchDown;
}

EngineStartButton::EngineStartButton(uint16_t cx, uint16_t cy) : CircleButton(cx, cy, ENGINE_START_BUTTON_OUTER_RADIUS) {}

void EngineStartButton::draw() {
    uint16_t widthEngine, heightEngine, widthStart, heightStart;
    int16_t tempX, tempY;

    TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_OUTER_RADIUS, TFT.color565(200, 200, 200));
    TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_MIDDLE_RADIUS, TFT.color565(150, 150, 150));
    TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS, TFT.color565(30, 30, 30));

    TFT.setTextSize(ENGINE_START_BUTTON_ENGINE_TEXT_SIZE);
    TFT.getTextBounds("ENGINE", 0, 0, &tempX, &tempY, &widthEngine, &heightEngine);
    TFT.setCursor(cx - widthEngine / 2, cy + ENGINE_START_BUTTON_ENGINE_TEXT_Y);
    TFT.print("ENGINE");
    TFT.setTextSize(ENGINE_START_BUTTON_START_TEXT_SIZE);
    TFT.getTextBounds("START", 0, 0, &tempX, &tempY, &widthStart, &heightStart);
    TFT.setCursor(cx - widthStart / 2, cy + ENGINE_START_BUTTON_START_TEXT_Y);
    TFT.print("START");

    TFT.fillRoundRect(cx - ENGINE_START_BUTTON_INDICATOR_WIDTH / 2, cy + ENGINE_START_BUTTON_INDICATOR_Y, ENGINE_START_BUTTON_INDICATOR_WIDTH, ENGINE_START_BUTTON_INDICATOR_HEIGHT, ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS, enabled ? GREEN : RED);
}

TextButton::TextButton(uint16_t cx, uint16_t cy, uint16_t r, char *str, uint8_t textSize) : CircleButton(cx, cy, r), str(str), textSize(textSize) {
    int16_t tempX, tempY;

    TFT.setTextSize(textSize);
    TFT.getTextBounds(str, 0, 0, &tempX, &tempY, &textWidth, &textHeight);
}

void TextButton::draw() {
    TFT.drawCircle(cx, cy, r, WHITE);
    TFT.setCursor(cx - textWidth / 2, cy - textHeight / 2);
    TFT.setTextSize(textSize);
    TFT.print(str);
}
