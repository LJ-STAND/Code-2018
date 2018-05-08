#ifndef BUTTON_H
#define BUTTON_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <Config.h>
#include <Common.h>

class Button {
public:
    Button() {}
    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void draw() {}
    void checkDraw();
    void setNeedsDraw(bool highlightOnly = false);

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);
    void setEnabled(bool isEnabled);
    bool getEnabled();

protected:
    uint16_t x, y, w, h;
    bool highlighted;
    bool enabled;
    bool needsDrawing = true;
    bool needsHighlightOnly = false;
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
};

class HomeButton : public CircleButton {
public:
    HomeButton() {}
    HomeButton(uint16_t cx, uint16_t cy);

    void draw();
};

class BackButton : public Button {
public:
    BackButton() {}
    BackButton(uint16_t x, uint16_t y);

    void draw();
};

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

class CheckBox : public Button {
public:
    CheckBox() {}
    CheckBox(uint16_t x, uint16_t y);

    void draw();
};

#endif // BUTTON_H
