#include <Arduino.h>
#include <SPI.h>

#define CSPIN 43
#define CSPIN1 43
#define CSPIN2 55
#define CSPIN3 54
#define MISO 45
#define MOSI 44
#define SCLK 46
// //
// uint8_t transmitSPI(uint8_t data)
// {
//    // select device (active low)
//    digitalWrite(CSPIN, LOW);
//
//    // send bits 7..0
//    for (int i = 0; i < 8; i++) {
//        // consider leftmost bit
//        // set line high if bit is 1, low if bit is 0
//        digitalWrite(MOSI, (data >> i) & 0x1 == 0x1 ? HIGH : LOW);
//
//        delay(1);
//        // pulse clock to indicate that bit value should be read
//        digitalWrite(SCLK, HIGH);
//        delay(1);
//        digitalWrite(SCLK, LOW);
//    }
//
//    delay(10);
//
//    uint8_t result;
//
//    for (int i = 0; i < 8; i++) {
//         result |= digitalRead(MISO) << (7 - i);
//         delay(1);
//         digitalWrite(SCLK, HIGH);
//         delay(1);
//         digitalWrite(SCLK, LOW);
//    }
//
//    // deselect device
//    digitalWrite(CSPIN, HIGH);
//
//    return result;
// }
//
// uint8_t read() {
//     uint8_t result = 0;
//
//     for (int i = 0; i < 8; i++) {
//          result = result | (digitalRead(MISO) << (7 - i));
//          delay(1);
//          digitalWrite(SCLK, HIGH);
//          delay(1);
//          digitalWrite(SCLK, LOW);
//     }
//
//     // deselect device
//     digitalWrite(CSPIN, HIGH);
//
//     return result;
// }

void setup() {
    Serial.begin(9600);



    pinMode(CSPIN1, OUTPUT);
    pinMode(CSPIN2, OUTPUT);
    pinMode(CSPIN3, OUTPUT);

    digitalWrite(CSPIN1, HIGH);
    digitalWrite(CSPIN2, HIGH);
    digitalWrite(CSPIN3, HIGH);
    // pinMode(SCLK, OUTPUT);
    // pinMode(MOSI, OUTPUT);
    // pinMode(MISO, INPUT);
    // // digitalWrite(CSPIN, HIGH);

    // digitalWrite(CSPIN1, LOW);
    // SPI2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    // SPI2.transfer((0x13 << 8) | 0x2);
    // SPI2.endTransaction();
    // digitalWrite(CSPIN1, HIGH);

    // digitalWrite(CSPIN2, LOW);
    // SPI2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    // SPI2.transfer((0x13 << 8) | 0x2);
    // SPI2.endTransaction();
    // digitalWrite(CSPIN2, HIGH);
    //
    // digitalWrite(CSPIN3, LOW);
    // SPI2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    // SPI2.transfer((0x13 << 8) | 0x2);
    // SPI2.endTransaction();
    // digitalWrite(CSPIN3, HIGH);

    SPI2.begin();
}

void loop() {

    digitalWrite(CSPIN1, LOW);
    // digitalWrite(CSPIN2, HIGH);
    // digitalWrite(CSPIN3, HIGH);
    SPI2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    //
    Serial.println(SPI2.transfer(0x80 | 0xC));
    //
    //
    SPI2.endTransaction();
    // delay(10);
    digitalWrite(CSPIN1, HIGH);
    // digitalWrite(CSPIN2, LOW);
    // digitalWrite(CSPIN3, LOW);
    //
    // Serial.println(transmitSPI(0x8C));
    delay(50);

}
