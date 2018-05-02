#ifndef BUTTON_H
#define BUTTON_H

#include <Adafruit_ILI9341_8bit.h>
#include <Config.h>
#include <Common.h>

class Button {
public:
    Button() {}
    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void draw();

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);

protected:
    uint16_t x, y, w, h;
    bool highlighted;
};

class CircleButton : public Button {
public:
    CircleButton() {}
    CircleButton(uint16_t cx, uint16_t cy, uint16_t r);

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);

protected:
    uint16_t cx, cy;
    uint16_t r;
};

class EngineStartButton : public CircleButton {
public:
    EngineStartButton() {}
    EngineStartButton(uint16_t cx, uint16_t cy);

    void draw();

    bool enabled;
};

// class HomeButton : public CircleButton {
// public:
//     HomeButton() {}
//     HomeButton(uint16_t cx, uint16_t cy);
// };

class TextButton : public CircleButton {
public:
    TextButton() {}
    TextButton(uint16_t cx, uint16_t cy, uint16_t r, char *str, uint8_t textSize);

    void draw();

protected:
    char *str;
    uint8_t textSize;

    uint16_t textWidth, textHeight;
};

#endif // BUTTON_H
