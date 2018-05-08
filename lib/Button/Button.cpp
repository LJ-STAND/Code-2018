#include "Button.h"

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : x(x), y(y), w(w), h(h) {}

CircleButton::CircleButton(uint16_t cx, uint16_t cy, uint16_t r) : Button(cx - r, cy - r, r * 2, r * 2), cx(cx), cy(cy), r(r) {}

void Button::setEnabled(bool isEnabled) {
    if (enabled != isEnabled) {
        enabled = isEnabled;
        setNeedsDraw(true);
    }
}

bool Button::getEnabled() {
    return enabled;
}

void Button::checkDraw() {
    if (needsDrawing) {
        draw();
        needsDrawing = false;
    }
}

void Button::setNeedsDraw(bool highlightOnly) {
    needsDrawing = true;
    needsHighlightOnly = highlightOnly;
}

bool Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = (tx > x) && (tx < x + w) && (ty > y) && (ty < y + h);

    if (inButton && touchDown != highlighted) {
        highlighted = inButton && touchDown;
        setNeedsDraw(true);
    }

    return inButton && !touchDown;
}

bool CircleButton::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool inButton = distanceBetween(cx, tx, cy, ty) < r;

    if ((inButton && touchDown) != highlighted) {
        highlighted = inButton && touchDown;
        setNeedsDraw(true);
    }

    return inButton && !touchDown;
}

EngineStartButton::EngineStartButton(uint16_t cx, uint16_t cy) : CircleButton(cx, cy, ENGINE_START_BUTTON_OUTER_RADIUS) {}

void EngineStartButton::draw() {
    if (needsHighlightOnly) {
        TFT.fillRoundRect(cx - ENGINE_START_BUTTON_INDICATOR_WIDTH / 2, cy + ENGINE_START_BUTTON_INDICATOR_Y, ENGINE_START_BUTTON_INDICATOR_WIDTH, ENGINE_START_BUTTON_INDICATOR_HEIGHT, ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS, enabled ? GREEN : RED);

        TFT.drawCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS - 3, highlighted ? TFT.color565(150, 150, 150) : TFT.color565(30, 30, 30));
    } else {
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

        TFT.drawCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS - 3, highlighted ? TFT.color565(150, 150, 150) : TFT.color565(30, 30, 30));
    }
}

TextButton::TextButton(uint16_t cx, uint16_t cy, uint16_t r, char *str, uint8_t textSize) : CircleButton(cx, cy, r), str(str), textSize(textSize) {
    int16_t tempX, tempY;

    TFT.setTextSize(textSize);
    TFT.getTextBounds(str, 0, 0, &tempX, &tempY, &textWidth, &textHeight);
}

void TextButton::draw() {
    TFT.drawCircle(cx, cy, r, highlighted ? TFT.color565(150, 150, 150) : WHITE);
    TFT.setCursor(cx - textWidth / 2, cy - textHeight / 2);
    TFT.setTextSize(textSize);
    TFT.print(str);
}

HomeButton::HomeButton(uint16_t cx, uint16_t cy) : CircleButton(cx, cy, HOME_BUTTON_OUTER_RADIUS * 3) {}

void HomeButton::draw() {
    TFT.drawCircle(cx, cy, HOME_BUTTON_OUTER_RADIUS, WHITE);
    TFT.fillCircle(cx, cy, HOME_BUTTON_INNER_RADIUS, highlighted ? BLACK : WHITE);
}

BackButton::BackButton(uint16_t x, uint16_t y) : Button(x, y, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT) {}

void BackButton::draw() {
    TFT.drawLine(x, y + h / 2, x + w, y + h / 2, WHITE);
}

void CheckBox::draw() {
    TFT.drawRect(x, y, CHECK_BOX_OUTER_SIZE, CHECK_BOX_OUTER_SIZE, highlighted ? TFT.color565(150, 150, 150) : WHITE);
    TFT.fillRect(x + (CHECK_BOX_OUTER_SIZE - CHECK_BOX_INNER_SIZE) / 2, y + (CHECK_BOX_OUTER_SIZE - CHECK_BOX_INNER_SIZE) / 2, CHECK_BOX_INNER_SIZE, CHECK_BOX_INNER_SIZE, enabled ? WHITE : BLACK);

}

CheckBox::CheckBox(uint16_t x, uint16_t y) : Button(x, y, CHECK_BOX_OUTER_SIZE, CHECK_BOX_OUTER_SIZE) {}
