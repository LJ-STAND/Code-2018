#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include <Pins.h>
#include <Config.h>
#include <Common.h>

class LED {
public:
    void init();

    void rainbow();
    uint32_t HSBToRGB(float hue, float saturation, float brightness);
    void displayAngle(uint16_t angle, double hue);
    void displayPlayMode(bool isAttacker);
    void rgbColor(uint32_t color);

    Adafruit_NeoPixel rgb = Adafruit_NeoPixel(RGB_LED_COUNT, DEBUG_RGB_LED, NEO_GRB + NEO_KHZ800);

private:
    unsigned long delayTimerLastReading = 0;
    uint16_t loopCounter = 0;
};

#endif // LED_H
