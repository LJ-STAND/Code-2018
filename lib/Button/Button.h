#ifndef BUTTON_H
#define BUTTON_H

#include <Adafruit_ILI9341_8bit.h>

class Button {
public:
    Button() {}
    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_ILI9341_8bit *tft);

    void draw();

    bool isTouched(uint16_t tx, uint16_t ty, bool touchDown);

private:
    uint8_t x, y, w, h;
    Adafruit_ILI9341_8bit *tft;
    bool highlighted;
};

class EngineStartButton : public Button {
private:
}

#endif // BUTTON_H
