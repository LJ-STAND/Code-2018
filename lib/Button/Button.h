#ifndef BUTTON_H
#define BUTTON_H

#include <Adafruit_ILI9341_8bit.h>
#include <Config.h>

class Button {
public:
    Button() {}
    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_ILI9341_8bit &tft);

    void draw();

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);

    uint16_t x, y, w, h;
    Adafruit_ILI9341_8bit *tft;
    bool highlighted;
};

class EngineStartButton : public Button {
public:
    EngineStartButton() {}
    EngineStartButton(uint16_t x, uint16_t y, Adafruit_ILI9341_8bit &tft);

    void draw();

    bool enabled;
};

class TextButton : public Button {
public:
    TextButton() {}
    TextButton(uint16_t x, uint16_t y, uint16_t r, String str, uint8_t textSize, Adafruit_ILI9341_8bit &screen);

    void draw();

    uint16_t r;

    String str;
    uint8_t textSize;

    uint16_t textWidth, textHeight;
};

#endif // BUTTON_H
