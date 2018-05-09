#ifndef SCREEN_UI_H
#define SCREEN_UI_H

#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <Config.h>
#include <Common.h>
#include <Fonts/Org_01.h>

class View {
public:
    View() {}
    View(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void draw() {}
    void checkDraw();
    virtual void setNeedsDraw();

    uint16_t x, y, w, h;

protected:
    bool needsDrawing = true;
};

class Dial : public View {
public:
    Dial() {}
    Dial(uint16_t cx, uint16_t cy, uint16_t r, int maxValue, double minAngle, double maxAngle);

    void draw();
    void setValue(int newValue);
    int getValue();

protected:
    double minAngle, maxAngle;
    int maxValue;
    int value;
    int oldValue;

    uint16_t cx, cy, r;
};

class Button : public View {
public:
    Button() {}
    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void setNeedsDraw(bool highlightOnly = false);

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);
    virtual bool inButton(uint16_t tx, uint16_t ty);

    void setEnabled(bool isEnabled);
    bool getEnabled();

protected:
    bool highlighted = false;
    bool enabled = false;
    bool needsHighlightOnly = false;
};

class Label : public View {
public:
    Label() {}
    Label(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *str, uint8_t textSize);

    void draw();
    void setText(char *newStr);

protected:
    char *str;
    uint8_t textSize;
};

class CircleButton : public Button {
public:
    CircleButton() {}
    CircleButton(uint16_t cx, uint16_t cy, uint16_t r);

    bool inButton(uint16_t tx, uint16_t ty);

protected:
    uint16_t cx, cy, r;
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
