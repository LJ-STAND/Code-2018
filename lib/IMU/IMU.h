#ifndef IMU_H
#define IMU_H

#include <SPI.h>
#include <Arduino.h>
#include <Pins.h>
#include <Config.h>
#include <Common.h>
#include <EEPROM.h>

#define GYRO_CONFIG_REGISTER 0x1B
#define PWR_MGMNT_1_REGISTER 0x6B
#define PWR_MGMNT_2_REGISTER 0x6C
#define CONFIG_REGISTER 0x1A
#define SMPLRT_DIV_REGISTER 0x19

#define GYRO_DLPF_184 0x01
#define CLOCK_SEL_PLL 0x01
#define SEN_ENABLE 0x00
#define GYRO_RANGE_250DPS 0x00
#define GYRO_RANGE_500DPS 0x08
#define GYRO_RANGE_1000DPS 0x10
#define GYRO_RANGE_2000DPS 0x18
#define SPI_READ 0x80

class IMU {
public:
    IMU() {}
    IMU(SPIClass &spi, int cs, int eeprom) {
        spiClass = &spi;
        csPin = cs;
        eepromAddress = eeprom;
    }

    void init();
    void calibrate();
    void setGyroRange(uint8_t range);

    int16_t readRawGyroZ();
    double readGyroZ();

private:
    void writeRegister(uint8_t address, uint8_t data);
    void readRegisters(uint8_t address, uint8_t count, uint8_t *buffer);

    int16_t calibration = 0;
    double gyroScale = 0;

    int eepromAddress;

    SPIClass *spiClass;
    int csPin;
};

class IMUFusion {
public:
    void init();

    void update();
    void calibrate();
    double getHeading();
    void resetHeading();

    IMU imu1 = IMU(SPI2, IMU_1_CS, IMU_1_CALIBRATION_EEPROM);
    IMU imu2 = IMU(SPI2, IMU_2_CS, IMU_2_CALIBRATION_EEPROM);
    IMU imu3 = IMU(SPI2, IMU_3_CS, IMU_3_CALIBRATION_EEPROM);

private:
    double heading = 0;
    unsigned long previousTime;
};

#endif // IMU_H
