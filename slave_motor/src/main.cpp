#include <MotorArray.h>
#include <Arduino.h>
#include <Pins.h>
#include <PID.h>
#include <t3spi.h>
#include <Timer.h>

MotorArray motors;

volatile uint16_t dataIn[1];
volatile uint16_t dataOut[1];

volatile int speed;
volatile int angle;

T3SPI spi;

Timer ledTimer = Timer(1000000);
bool ledOn;

void setup() {
    analogWriteResolution(16);

    motors.init();

    spi.begin_SLAVE(ALT_SCK, MOSI, MISO, CS0);
    spi.setCTAR_SLAVE(16, SPI_MODE0);

    NVIC_ENABLE_IRQ(IRQ_SPI0);

    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    motors.move(angle, 0, speed);
    motors.update();

    if (ledTimer.timeHasPassed()) {
        digitalWrite(LED_BUILTIN, ledOn);
        ledOn = !ledOn;
    }
}

void spi0_isr() {
    spi.rxtx16(dataIn, dataOut, 1);

    if (dataIn[0] != 0) {
        if ((dataIn[0] & 0x8000) == 0x8000) {
            speed = dataIn[0] & (~0x8000);
        } else {
            angle = dataIn[0] & (~0x8000);
        }
    }
}
