#include <Arduino.h>
#include <Pins.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ILI9341_8bit.h>
#include <TouchScreen.h>
#include <SPI.h>
#include <MovingAverage.h>

#define YP A3
#define XM A2
#define YM 18
#define XP 15

#define TS_MINX  150
#define TS_MINY  90
#define TS_MAXX  905
#define TS_MAXY  890

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_ILI9341_8bit tft;

#define BOXSIZE   40
#define PENRADIUS  1
int oldcolor, currentcolor;

#define LED_PIN 23
#define LED_COUNT 12
#define LED_STATE_COUNT 10

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

volatile uint8_t ledStateCounter = 0;

unsigned long delayTimerLastReading = 0;
unsigned long delayTimer2LastReading = 0;
uint16_t loopCounter = 0;
uint16_t loopCounter2 = 0;

bool buttonPressed;
//
// void setup() {
//     leds.begin();
//     leds.setBrightness(40);
//
//     pinMode(BUTTON_PIN, INPUT);
//     pinMode(LED_PIN, OUTPUT);
//
//     pinMode(DEBUG_LED_RED, OUTPUT);
//     pinMode(DEBUG_LED_GREEN, OUTPUT);
//     pinMode(DEBUG_LED_BLUE, OUTPUT);
//     pinMode(DEBUG_LED_ORANGE, OUTPUT);
//     pinMode(DEBUG_LED_YELLOW, OUTPUT);
//     pinMode(DEBUG_LED_WHITE, OUTPUT);
//     pinMode(DEBUG_LED_ATTACKER, OUTPUT);
//     pinMode(DEBUG_LED_DEFENDER, OUTPUT);
//
//     digitalWrite(DEBUG_LED_RED, HIGH);
//     digitalWrite(DEBUG_LED_GREEN, HIGH);
//     digitalWrite(DEBUG_LED_BLUE, HIGH);
//     digitalWrite(DEBUG_LED_ORANGE, HIGH);
//     digitalWrite(DEBUG_LED_YELLOW, HIGH);
//     digitalWrite(DEBUG_LED_WHITE, HIGH);
//
//     digitalWrite(DEBUG_LED_ATTACKER, HIGH);
//     digitalWrite(DEBUG_LED_DEFENDER, HIGH);
// }
//
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

void colorWipe(uint32_t color) {
    if (loopCounter < LED_COUNT) {
        if (millis() - delayTimerLastReading >= 50 || loopCounter == 0) {
            leds.setPixelColor(loopCounter, color);
            leds.show();

            delayTimerLastReading = millis();
            loopCounter++;
        }
    }
}
//
// void loop() {
//     switch (ledStateCounter) {
//         case 0:
//         if (millis() - delayTimerLastReading >= 80) {
//             for (uint8_t i = 0; i < LED_COUNT; i++) {
//                 leds.setPixelColor(i, HSBToRGB((float)((i + loopCounter) % LED_COUNT) / (float)LED_COUNT * 360, 1.0, 1.0));
//             }
//
//             leds.show();
//
//             delayTimerLastReading = millis();
//
//             if (loopCounter < LED_COUNT - 1) {
//                 loopCounter++;
//             } else {
//                 loopCounter = 0;
//             }
//         }
//
//         break;
//
//         case 1:
//         if (millis() - delayTimerLastReading >= 30) {
//             for (uint8_t i = 0; i < LED_COUNT; i++) {
//                 leds.setPixelColor(i, HSBToRGB(loopCounter, 1.0, 1.0));
//             }
//
//             leds.show();
//
//             delayTimerLastReading = millis();
//
//             if (loopCounter < 360) {
//                 loopCounter++;
//             } else {
//                 loopCounter = 0;
//             }
//         }
//
//         break;
//
//         case 2:
//         colorWipe(leds.Color(255, 255, 255));
//
//         break;
//
//         case 3:
//         colorWipe(leds.Color(255, 0, 0));
//
//         break;
//
//         case 4:
//         colorWipe(leds.Color(0, 255, 0));
//
//         break;
//
//         case 5:
//         colorWipe(leds.Color(0, 0, 255));
//
//         break;
//
//         case 6:
//         colorWipe(HSBToRGB(315, 1.0, 1.0));
//
//         break;
//
//         case 7:
//         if (millis() - delayTimerLastReading >= 10) {
//             uint16_t hue = loopCounter2 / 5;
//             uint16_t complimentaryHue = (hue + 180) % 360;
//             if (loopCounter < 255) {
//                 for (uint8_t i = 0; i < LED_COUNT; i++) {
//                     if (i % 2 == 0) {
//                         leds.setPixelColor(i, HSBToRGB(hue, 1.0, (float)(255 - loopCounter) / 255.0));
//                     } else {
//                         leds.setPixelColor(i, HSBToRGB(complimentaryHue, 1.0, (float)loopCounter / 255.0));
//                     }
//                 }
//
//                 loopCounter++;
//             } else if (loopCounter < 510) {
//                 for (uint8_t i = 0; i < LED_COUNT; i++) {
//                     if (i % 2 == 0) {
//                         leds.setPixelColor(i, HSBToRGB(hue, 1.0, (float)(loopCounter - 255) / 255.0));
//                     } else {
//                         leds.setPixelColor(i, HSBToRGB(complimentaryHue, 1.0, (float)(510 - loopCounter) / 255.0));
//                     }
//                 }
//
//                 loopCounter++;
//             } else {
//                 loopCounter = 0;
//             }
//
//             leds.show();
//
//             if (loopCounter2 < 1799) {
//                 loopCounter2++;
//             } else {
//                 loopCounter2 = 0;
//             }
//         }
//
//         break;
//
//         case 8:
//         if (millis() - delayTimerLastReading >= 60) {
//             for (uint8_t i = 0; i < LED_COUNT; i++) {
//                 leds.setPixelColor(i, HSBToRGB(315, 1.0, min(abs((float)((i + loopCounter) % LED_COUNT) / LED_COUNT - 0.5) + 0.1, 1.0)));
//             }
//
//             leds.show();
//
//             delayTimerLastReading = millis();
//
//             if (loopCounter < LED_COUNT - 1) {
//                 loopCounter++;
//             } else {
//                 loopCounter = 0;
//             }
//         }
//
//         break;
//
//         case 9:
//         if (millis() - delayTimerLastReading >= 4000) {
//             loopCounter = 0;
//             if (loopCounter2 < 5) {
//                 loopCounter2++;
//             } else {
//                 loopCounter2 = 0;
//             }
//         }
//
//         if (loopCounter <= 8) {
//             if (millis() - delayTimerLastReading >= 80 || loopCounter == 0) {
//                 leds.setPixelColor(8 + min(loopCounter, LED_COUNT - 1), HSBToRGB(loopCounter2 * 60, 1.0, 1.0));
//                 leds.setPixelColor(8 - loopCounter, HSBToRGB(loopCounter2 * 60, 1.0, 1.0));
//
//                 leds.show();
//
//                 delayTimerLastReading = millis();
//                 loopCounter++;
//             }
//         }
//
//         break;
//     }
//
//     if (buttonPressed) {
//         buttonPressed = false;
//         loopCounter = 0;
//         loopCounter2 = 0;
//         delayTimerLastReading = millis();
//         delayTimer2LastReading = millis();
//     }
// }

//-
// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341_8bit.h>
//
// #define	BLACK   0x0000
// #define	BLUE    0x001F
// #define	RED     0xF800
// #define	GREEN   0x07E0
// #define CYAN    0x07FF
// #define MAGENTA 0xF81F
// #define YELLOW  0xFFE0
// #define WHITE   0xFFFF
//
// Adafruit_ILI9341_8bit_STM tft = Adafruit_ILI9341_8bit_STM();
//
// unsigned long testFillScreen() {
//   unsigned long start = micros();
//   tft.fillScreen(BLACK);
//   tft.fillScreen(RED);
//   tft.fillScreen(GREEN);
//   tft.fillScreen(BLUE);
//   tft.fillScreen(BLACK);
//   return micros() - start;
// }
//
// unsigned long testText() {
//   tft.fillScreen(BLACK);
//   unsigned long start = micros();
//   tft.setCursor(0, 0);
//   tft.setTextColor(WHITE);  tft.setTextSize(6);
//   tft.println("LJ STAND");
//   // tft.setTextColor(YELLOW); tft.setTextSize(2);
//   // tft.println(1234.56);
//   // tft.setTextColor(RED);    tft.setTextSize(3);
//   // tft.println(0xDEADBEEF, HEX);
//   // tft.println();
//   // tft.setTextColor(GREEN);
//   // tft.setTextSize(5);
//   // tft.println("Groop");
//   // tft.setTextSize(2);
//   // tft.println("I implore thee,");
//   // tft.setTextSize(1);
//   // tft.println("my foonting turlingdromes.");
//   // tft.println("And hooptiously drangle me");
//   // tft.println("with crinkly bindlewurdles,");
//   // tft.println("Or I will rend thee");
//   // tft.println("in the gobberwarts");
//   // tft.println("with my blurglecruncheon,");
//   // tft.println("see if I don't!");
//   return micros() - start;
// }
//
// unsigned long testLines(uint16_t color) {
//   unsigned long start, t;
//   int           x1, y1, x2, y2,
//                 w = tft.width(),
//                 h = tft.height();
//
//   tft.fillScreen(BLACK);
//
//   x1 = y1 = 0;
//   y2    = h - 1;
//   start = micros();
//   for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   x2    = w - 1;
//   for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   t     = micros() - start; // fillScreen doesn't count against timing
//
//   tft.fillScreen(BLACK);
//
//   x1    = w - 1;
//   y1    = 0;
//   y2    = h - 1;
//   start = micros();
//   for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   x2    = 0;
//   for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   t    += micros() - start;
//
//   tft.fillScreen(BLACK);
//
//   x1    = 0;
//   y1    = h - 1;
//   y2    = 0;
//   start = micros();
//   for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   x2    = w - 1;
//   for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   t    += micros() - start;
//
//   tft.fillScreen(BLACK);
//
//   x1    = w - 1;
//   y1    = h - 1;
//   y2    = 0;
//   start = micros();
//   for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
//   x2    = 0;
//   for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
//
//   return micros() - start;
// }
//
// unsigned long testFastLines(uint16_t color1, uint16_t color2) {
//   unsigned long start;
//   int           x, y, w = tft.width(), h = tft.height();
//
//   tft.fillScreen(BLACK);
//   start = micros();
//   for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
//   for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);
//
//   return micros() - start;
// }
//
// unsigned long testRects(uint16_t color) {
//   unsigned long start;
//   int           n, i, i2,
//                 cx = tft.width()  / 2,
//                 cy = tft.height() / 2;
//
//   tft.fillScreen(BLACK);
//   n     = min(tft.width(), tft.height());
//   start = micros();
//   for(i=2; i<n; i+=6) {
//     i2 = i / 2;
//     tft.drawRect(cx-i2, cy-i2, i, i, color);
//   }
//
//   return micros() - start;
// }
//
// unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
//   unsigned long start, t = 0;
//   int           n, i, i2,
//                 cx = tft.width()  / 2 - 1,
//                 cy = tft.height() / 2 - 1;
//
//   tft.fillScreen(BLACK);
//   n = min(tft.width(), tft.height());
//   for(i=n; i>0; i-=6) {
//     i2    = i / 2;
//     start = micros();
//     tft.fillRect(cx-i2, cy-i2, i, i, color1);
//     t    += micros() - start;
//     // Outlines are not included in timing results
//     tft.drawRect(cx-i2, cy-i2, i, i, color2);
//   }
//
//   return t;
// }
//
// unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
//   unsigned long start;
//   int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;
//
//   tft.fillScreen(BLACK);
//   start = micros();
//   for(x=radius; x<w; x+=r2) {
//     for(y=radius; y<h; y+=r2) {
//       tft.fillCircle(x, y, radius, color);
//     }
//   }
//
//   return micros() - start;
// }
//
// unsigned long testCircles(uint8_t radius, uint16_t color) {
//   unsigned long start;
//   int           x, y, r2 = radius * 2,
//                 w = tft.width()  + radius,
//                 h = tft.height() + radius;
//
//   // Screen is not cleared for this one -- this is
//   // intentional and does not affect the reported time.
//   start = micros();
//   for(x=0; x<w; x+=r2) {
//     for(y=0; y<h; y+=r2) {
//       tft.drawCircle(x, y, radius, color);
//     }
//   }
//
//   return micros() - start;
// }
//
// unsigned long testTriangles() {
//   unsigned long start;
//   int           n, i, cx = tft.width()  / 2 - 1,
//                       cy = tft.height() / 2 - 1;
//
//   tft.fillScreen(BLACK);
//   n     = min(cx, cy);
//   start = micros();
//   for(i=0; i<n; i+=5) {
//     tft.drawTriangle(
//       cx    , cy - i, // peak
//       cx - i, cy + i, // bottom left
//       cx + i, cy + i, // bottom right
//       tft.color565(0, 0, i));
//   }
//
//   return micros() - start;
// }
//
// unsigned long testFilledTriangles() {
//   unsigned long start, t = 0;
//   int           i, cx = tft.width()  / 2 - 1,
//                    cy = tft.height() / 2 - 1;
//
//   tft.fillScreen(BLACK);
//   start = micros();
//   for(i=min(cx,cy); i>10; i-=5) {
//     start = micros();
//     tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
//       tft.color565(0, i, i));
//     t += micros() - start;
//     tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
//       tft.color565(i, i, 0));
//   }
//
//   return t;
// }
//
// unsigned long testRoundRects() {
//   unsigned long start;
//   int           w, i, i2,
//                 cx = tft.width()  / 2 - 1,
//                 cy = tft.height() / 2 - 1;
//
//   tft.fillScreen(BLACK);
//   w     = min(tft.width(), tft.height());
//   start = micros();
//   for(i=0; i<w; i+=6) {
//     i2 = i / 2;
//     tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
//   }
//
//   return micros() - start;
// }
//
// unsigned long testFilledRoundRects() {
//   unsigned long start;
//   int           i, i2,
//                 cx = tft.width()  / 2 - 1,
//                 cy = tft.height() / 2 - 1;
//
//   tft.fillScreen(BLACK);
//   start = micros();
//   for(i=min(tft.width(), tft.height()); i>20; i-=6) {
//     i2 = i / 2;
//     tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
//   }
//
//   return micros() - start;
// }
//
//
// void setup(void) {
//
//   Serial.begin(9600);
//   Serial.println(F("TFT LCD test"));
//
//   tft.begin();
//
//   Serial.println(F("Benchmark                Time (microseconds)"));
//
//   Serial.print(F("Screen fill              "));
//   Serial.println(testFillScreen());
//   delay(500);
//
//   Serial.print(F("Text                     "));
//   Serial.println(testText());
//   delay(3000);
//
//   Serial.print(F("Lines                    "));
//   Serial.println(testLines(CYAN));
//   delay(500);
//
//   Serial.print(F("Horiz/Vert Lines         "));
//   Serial.println(testFastLines(RED, BLUE));
//   delay(500);
//
//   Serial.print(F("Rectangles (outline)     "));
//   Serial.println(testRects(GREEN));
//   delay(500);
//
//   Serial.print(F("Rectangles (filled)      "));
//   Serial.println(testFilledRects(YELLOW, MAGENTA));
//   delay(500);
//
//   Serial.print(F("Circles (filled)         "));
//   Serial.println(testFilledCircles(10, MAGENTA));
//
//   Serial.print(F("Circles (outline)        "));
//   Serial.println(testCircles(10, WHITE));
//   delay(500);
//
//   Serial.print(F("Triangles (outline)      "));
//   Serial.println(testTriangles());
//   delay(500);
//
//   Serial.print(F("Triangles (filled)       "));
//   Serial.println(testFilledTriangles());
//   delay(500);
//
//   Serial.print(F("Rounded rects (outline)  "));
//   Serial.println(testRoundRects());
//   delay(500);
//
//   Serial.print(F("Rounded rects (filled)   "));
//   Serial.println(testFilledRoundRects());
//   delay(500);
//
//   Serial.println(F("Done!"));
// }
//
// void loop(void) {
//   for(uint8_t rotation=0; rotation<4; rotation++) {
//     tft.setRotation(rotation);
//     testText();
//     delay(2000);
//   }
// }

// Paint example specifically for the TFTLCD Arduino shield.
// If using the breakout board, use the tftpaint.pde sketch instead!

// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_ILI9341_8bit.h> // Hardware-specific library
// #include <TouchScreen.h>
//
// // These are the pins for the shield!
// #define YP A3  // must be an analog pin, use "An" notation!
// #define XM A2  // must be an analog pin, use "An" notation!
// #define YM 18   // can be a digital pin
// #define XP 15   // can be a digital pin
//
// #ifdef __SAM3X8E__
//   #define TS_MINX 125
//   #define TS_MINY 170
//   #define TS_MAXX 880
//   #define TS_MAXY 940
// #else
//   #define TS_MINX  150
//   #define TS_MINY  120
//   #define TS_MAXX  920
//   #define TS_MAXY  940
// #endif
//
// // For better pressure precision, we need to know the resistance
// // between X+ and X- Use any multimeter to read it
// // For the one we're using, its 300 ohms across the X plate
// TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//
// // Assign human-readable names to some common 16-bit color values:
// #define	BLACK   0x0000
// #define	BLUE    0x001F
// #define	RED     0xF800
// #define	GREEN   0x07E0
// #define CYAN    0x07FF
// #define MAGENTA 0xF81F
// #define YELLOW  0xFFE0
// #define WHITE   0xFFFF
//
//
// Adafruit_ILI9341_8bit tft;
//
// #define BOXSIZE   40
// #define PENRADIUS  4
// int oldcolor, currentcolor;

MovingAverage batteryAverage(50);

void setup(void) {
    pinMode(DEBUG_LED_RED, OUTPUT);
    pinMode(DEBUG_LED_GREEN, OUTPUT);
    pinMode(DEBUG_LED_BLUE, OUTPUT);
    pinMode(DEBUG_LED_ORANGE, OUTPUT);
    pinMode(DEBUG_LED_YELLOW, OUTPUT);
    pinMode(DEBUG_LED_WHITE, OUTPUT);
    pinMode(DEBUG_LED_ATTACKER, OUTPUT);
    pinMode(DEBUG_LED_DEFENDER, OUTPUT);

    digitalWrite(DEBUG_LED_RED, HIGH);
    digitalWrite(DEBUG_LED_GREEN, HIGH);
    digitalWrite(DEBUG_LED_BLUE, HIGH);
    digitalWrite(DEBUG_LED_ORANGE, HIGH);
    digitalWrite(DEBUG_LED_YELLOW, HIGH);
    digitalWrite(DEBUG_LED_WHITE, HIGH);

    digitalWrite(DEBUG_LED_ATTACKER, HIGH);
    digitalWrite(DEBUG_LED_DEFENDER, HIGH);

    leds.begin();
    leds.setBrightness(20);

    tft.begin();

    tft.setRotation(3);
    tft.fillScreen(BLACK);

    tft.setCursor(5, 0);
    tft.setTextSize(3);
    tft.print("LJ STAND");

    tft.drawRoundRect(tft.width() - 45, 5, 40, 20, 4, WHITE);
    tft.fillRect(tft.width() - 5, 10, 2, 10, WHITE);
    tft.fillRoundRect(tft.width() - 5, 10, 3, 10, 2, WHITE);

    tft.fillRoundRect(tft.width() - 41, 9, 32, 12, 1, GREEN);

    tft.drawLine(0, 30, tft.width(), 30, WHITE);

    uint16_t grey = tft.color565(150, 150, 150);
    uint16_t width1, height1, width2, height2;
    int16_t x, y;

    tft.fillCircle(tft.width() / 2, tft.height() / 2, 78, tft.color565(200, 200, 200));
    tft.fillCircle(tft.width() / 2, tft.height() / 2, 75, grey);
    tft.fillCircle(tft.width() / 2, tft.height() / 2, 70, tft.color565(30, 30, 30));
    tft.fillRoundRect(tft.width() / 2 - 20, tft.height() / 2 - 40, 40, 10, 3, RED);
    tft.setTextSize(2);
    tft.getTextBounds("ENGINE", 0, 0, &x, &y, &width1, &height1);
    tft.setCursor(tft.width() / 2 - width1 / 2, tft.height() / 2 - 10);
    tft.print("ENGINE");
    tft.setTextSize(3);
    tft.getTextBounds("START", 0, 0, &x, &y, &width2, &height2);
    tft.setCursor(tft.width() / 2 - width2 / 2, tft.height() / 2 + 10);
    tft.print("START");

    Serial.begin(9600);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define BATT_MULTIPILER 0.01767938931

bool on = false;
bool canPress = true;

void loop()
{

    switch (ledStateCounter) {
            case 0:
            if (millis() - delayTimerLastReading >= 80) {
                for (uint8_t i = 0; i < LED_COUNT; i++) {
                    leds.setPixelColor(i, HSBToRGB((float)((i + loopCounter) % LED_COUNT) / (float)LED_COUNT * 360, 1.0, 1.0));
                }

                leds.show();

                delayTimerLastReading = millis();

                if (loopCounter < LED_COUNT - 1) {
                    loopCounter++;
                } else {
                    loopCounter = 0;
                }
            }

            break;

            case 1:
            if (millis() - delayTimerLastReading >= 30) {
                for (uint8_t i = 0; i < LED_COUNT; i++) {
                    leds.setPixelColor(i, HSBToRGB(loopCounter, 1.0, 1.0));
                }

                leds.show();

                delayTimerLastReading = millis();

                if (loopCounter < 360) {
                    loopCounter++;
                } else {
                    loopCounter = 0;
                }
            }

            break;

            case 2:
            colorWipe(leds.Color(255, 255, 255));

            break;

            case 3:
            colorWipe(leds.Color(255, 0, 0));

            break;

            case 4:
            colorWipe(leds.Color(0, 255, 0));

            break;

            case 5:
            colorWipe(leds.Color(0, 0, 255));

            break;

            case 6:
            colorWipe(HSBToRGB(315, 1.0, 1.0));

            break;

            case 7:
            if (millis() - delayTimerLastReading >= 10) {
                uint16_t hue = loopCounter2 / 5;
                uint16_t complimentaryHue = (hue + 180) % 360;
                if (loopCounter < 255) {
                    for (uint8_t i = 0; i < LED_COUNT; i++) {
                        if (i % 2 == 0) {
                            leds.setPixelColor(i, HSBToRGB(hue, 1.0, (float)(255 - loopCounter) / 255.0));
                        } else {
                            leds.setPixelColor(i, HSBToRGB(complimentaryHue, 1.0, (float)loopCounter / 255.0));
                        }
                    }

                    loopCounter++;
                } else if (loopCounter < 510) {
                    for (uint8_t i = 0; i < LED_COUNT; i++) {
                        if (i % 2 == 0) {
                            leds.setPixelColor(i, HSBToRGB(hue, 1.0, (float)(loopCounter - 255) / 255.0));
                        } else {
                            leds.setPixelColor(i, HSBToRGB(complimentaryHue, 1.0, (float)(510 - loopCounter) / 255.0));
                        }
                    }

                    loopCounter++;
                } else {
                    loopCounter = 0;
                }

                leds.show();

                if (loopCounter2 < 1799) {
                    loopCounter2++;
                } else {
                    loopCounter2 = 0;
                }
            }

            break;

            case 8:
            if (millis() - delayTimerLastReading >= 60) {
                for (uint8_t i = 0; i < LED_COUNT; i++) {
                    leds.setPixelColor(i, HSBToRGB(315, 1.0, min(abs((float)((i + loopCounter) % LED_COUNT) / LED_COUNT - 0.5) + 0.1, 1.0)));
                }

                leds.show();

                delayTimerLastReading = millis();

                if (loopCounter < LED_COUNT - 1) {
                    loopCounter++;
                } else {
                    loopCounter = 0;
                }
            }

            break;

            case 9:
            if (millis() - delayTimerLastReading >= 4000) {
                loopCounter = 0;
                if (loopCounter2 < 5) {
                    loopCounter2++;
                } else {
                    loopCounter2 = 0;
                }
            }

            if (loopCounter <= 8) {
                if (millis() - delayTimerLastReading >= 80 || loopCounter == 0) {
                    leds.setPixelColor(8 + min(loopCounter, LED_COUNT - 1), HSBToRGB(loopCounter2 * 60, 1.0, 1.0));
                    leds.setPixelColor(8 - loopCounter, HSBToRGB(loopCounter2 * 60, 1.0, 1.0));

                    leds.show();

                    delayTimerLastReading = millis();
                    loopCounter++;
                }
            }

            break;
        }

        if (buttonPressed) {
            buttonPressed = false;
            loopCounter = 0;
            loopCounter2 = 0;
            delayTimerLastReading = millis();
            delayTimer2LastReading = millis();
        }

    TSPoint p = ts.getPoint();

    // if (p.z > MINPRESSURE && p.z < MAXPRESSURE) Serial.println(String(p.x) + " " + String(p.y));
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    //
    //
    Serial.println(String(p.x > 300 && p.y < 800) + ": " + String(p.x) + " " + String(p.y));

    // tft.setCursor(0, 0);
    // tft.fillRect(0, 0, 300, 30, BLACK);
    // tft.print(batteryAverage.average() * BATT_MULTIPILER);
    // batteryAverage.update(analogRead(V_BAT));

    // delay(100);

    if (p.x > 300 && p.y < 800) {
        if (canPress) {
            on = !on;
            buttonPressed = true;
            if (ledStateCounter < LED_STATE_COUNT) {
                ledStateCounter++;
            } else {
                ledStateCounter = 0;
            }

            canPress = false;
            tft.fillRoundRect(tft.width() / 2 - 20, tft.height() / 2 - 40, 40, 10, 3, on ? GREEN : RED);
        }

        // if (p.y < (TS_MINY+5)) {
        //     // press the bottom of the screen to erase
        //     tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
        // }
        // // scale from 0->1023 to tft.width
        // p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
        // p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
        //
        // if (p.y < BOXSIZE) {
        //     oldcolor = currentcolor;
        //
        //     if (p.x < BOXSIZE) {
        //         currentcolor = RED;
        //         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
        //     } else if (p.x < BOXSIZE*2) {
        //         currentcolor = YELLOW;
        //         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
        //     } else if (p.x < BOXSIZE*3) {
        //         currentcolor = GREEN;
        //         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
        //     } else if (p.x < BOXSIZE*4) {
        //         currentcolor = CYAN;
        //         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
        //     } else if (p.x < BOXSIZE*5) {
        //         currentcolor = BLUE;
        //         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
        //     } else if (p.x < BOXSIZE*6) {
        //         currentcolor = MAGENTA;
        //         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
        //     }
        //
        //     if (oldcolor != currentcolor) {
        //         if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
        //         if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
        //         if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
        //         if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
        //         if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
        //         if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
        //     }
        // }
        // if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
        //     tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
        // }
    } else {
        canPress = true;
    }
}
