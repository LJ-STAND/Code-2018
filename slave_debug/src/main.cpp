#include <Arduino.h>
#include <Pins.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ILI9341_8bit.h>
#include <Screen.h>
#include <SPI.h>
#include <MovingAverage.h>
#include <Config.h>

#define LED_STATE_COUNT 10

Adafruit_NeoPixel leds = Adafruit_NeoPixel(RGB_LED_COUNT, DEBUG_RGB_LED, NEO_GRB + NEO_KHZ800);

Screen screen;

unsigned long delayTimerLastReading = 0;
uint16_t loopCounter = 0;

uint32_t HSBToRGB(float hue, float saturation, float brightness) {
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

    return leds.Color(r, g, b);
}

void setup(void) {
    pinMode(DEBUG_LED_RED, OUTPUT);
    pinMode(DEBUG_LED_GREEN, OUTPUT);
    pinMode(DEBUG_LED_BLUE, OUTPUT);
    pinMode(DEBUG_LED_ORANGE, OUTPUT);
    pinMode(DEBUG_LED_YELLOW, OUTPUT);
    pinMode(DEBUG_LED_WHITE, OUTPUT);
    pinMode(DEBUG_LED_ATTACKER, OUTPUT);
    pinMode(DEBUG_LED_DEFENDER, OUTPUT);

    // digitalWrite(DEBUG_LED_RED, HIGH);
    // digitalWrite(DEBUG_LED_GREEN, HIGH);
    // digitalWrite(DEBUG_LED_BLUE, HIGH);
    // digitalWrite(DEBUG_LED_ORANGE, HIGH);
    // digitalWrite(DEBUG_LED_YELLOW, HIGH);
    // digitalWrite(DEBUG_LED_WHITE, HIGH);
    //
    // digitalWrite(DEBUG_LED_ATTACKER, HIGH);
    // digitalWrite(DEBUG_LED_DEFENDER, HIGH);

    // leds.begin();
    // leds.setBrightness(50);

    Serial.begin(9600);
    Serial5.begin(9600);

    screen.init();
}

bool on = false;
bool canPress = true;

void loop() {
    // if (millis() - delayTimerLastReading >= 80) {
    //     for (uint8_t i = 0; i < RGB_LED_COUNT; i++) {
    //         leds.setPixelColor(i, HSBToRGB((float)((i + loopCounter) % RGB_LED_COUNT) / (float)RGB_LED_COUNT * 360, 1.0, 1.0));
    //     }
    //
    //     leds.show();
    //
    //     delayTimerLastReading = millis();
    //
    //     if (loopCounter < RGB_LED_COUNT - 1) {
    //         loopCounter++;
    //     } else {
    //         loopCounter = 0;
    //     }
    // }

    screen.updateBatteryMeter();
}
