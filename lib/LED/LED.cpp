#include "LED.h"

void LED::init() {
    pinMode(DEBUG_LED_RED, OUTPUT);
    pinMode(DEBUG_LED_GREEN, OUTPUT);
    pinMode(DEBUG_LED_BLUE, OUTPUT);
    pinMode(DEBUG_LED_ORANGE, OUTPUT);
    pinMode(DEBUG_LED_YELLOW, OUTPUT);
    pinMode(DEBUG_LED_WHITE, OUTPUT);
    pinMode(DEBUG_LED_ATTACKER, OUTPUT);
    pinMode(DEBUG_LED_DEFENDER, OUTPUT);

    rgb.begin();
    rgb.setBrightness(30);
}

uint32_t LED::HSBToRGB(float hue, float saturation, float brightness) {
    float chroma = brightness * saturation;

    hue = constrain(hue, 0, 360) / 60.0;

    float x = chroma * (1 - abs(fmod(hue, 2.0) - 1.0));

    float r;
    float g;
    float b;

    if (hue <= 1) {
        r = chroma;
        g = x;
        b = 0;
    } else if (hue <= 2) {
        r = x;
        g = chroma;
        b = 0;
    } else if (hue <= 3) {
        r = 0;
        g = chroma;
        b = x;
    } else if (hue <= 4) {
        r = 0;
        g = x;
        b = chroma;
    } else if (hue <= 5) {
        r = x;
        g = 0;
        b = chroma;
    } else {
        r = chroma;
        g = 0;
        b = x;
    }

    float m = brightness - chroma;

    r = (r + m) * 255;
    g = (g + m) * 255;
    b = (b + m) * 255;

    return rgb.Color(r, g, b);
}

void LED::rainbow() {
    // if (millis() - delayTimerLastReading >= RAINBOW_TIME) {
    //     for (uint8_t i = 0; i < RGB_LED_COUNT; i++) {
    //         rgb.setPixelColor(i, HSBToRGB((float)((i + loopCounter) % RGB_LED_COUNT) / (float)RGB_LED_COUNT * 360, 1.0, 1.0));
    //     }

    //     rgb.show();

    //     delayTimerLastReading = millis();

    //     if (loopCounter < RGB_LED_COUNT - 1) {
    //         loopCounter++;
    //     } else {
    //         loopCounter = 0;
    //     }
    // }
}

void LED::displayAngle(uint16_t angle, double hue) {
    // for (uint8_t i = 0; i < RGB_LED_COUNT; i++) {
    //     rgb.setPixelColor(i, HSBToRGB(hue, 1.0, 1.0 - ((double)min(smallestAngleBetween(angle, i * 360 / RGB_LED_COUNT), 60) / 60.0)));
    // }

    // rgb.show();
}

void LED::displayAngleSize(uint16_t angle, double size, double maxSize, double minHue, double maxHue) {
    for (uint8_t i = 0; i < RGB_LED_COUNT; i++) {
        rgb.setPixelColor(i, HSBToRGB(minHue + (size / maxSize) * (maxHue - minHue), 1.0, 1.0 - ((double)min(smallestAngleBetween(angle, i * 360 / RGB_LED_COUNT), 60) / 60.0)));
    }

    rgb.show();
}

void LED::rgbColor(uint32_t color) {
    // for (uint8_t i = 0; i < RGB_LED_COUNT; i++) {
    //     rgb.setPixelColor(i, color);
    // }

    // rgb.show();
}

void LED::displayPlayMode(bool isAttacker) {
    digitalWrite(DEBUG_LED_DEFENDER, !isAttacker);
    digitalWrite(DEBUG_LED_ATTACKER, isAttacker);
}

void LED::rgbOff() {
    rgb.clear();
    rgb.show();
}