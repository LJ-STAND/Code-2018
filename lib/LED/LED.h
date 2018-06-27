#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include <Pins.h>
#include <Config.h>
#include <Common.h>

// What data the RGB leds show
enum RGBType {
    ballRGBType, // Ball angle and strength
    lineRGBType, // Line angle and strength
    rainbowRGBType, // RAINBOW!!
    goalRGBType // Goal angle
};
// Class to control the RGB and play mode LEDs
class LED {
public:
    void init();

    void rainbow();
    uint32_t HSBToRGB(float hue, float saturation, float brightness);
    void displayAngle(uint16_t angle, double hue);
    void displayAngleSize(uint16_t angle, double size, double maxSize, double minHue, double maxHue);
    void displayPlayMode(bool isAttacker);
    void rgbColor(uint32_t color);
    void rgbOff();

    Adafruit_NeoPixel rgb = Adafruit_NeoPixel(RGB_LED_COUNT, DEBUG_RGB_LED, NEO_GRB + NEO_KHZ800);

private:
    unsigned long delayTimerLastReading = 0; // Time counter for between LED loops
    uint16_t loopCounter = 0; // Loop counter of LED updates
};

#endif // LED_H
