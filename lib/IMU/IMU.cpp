#include "IMU.h"

void IMU::init() {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);

    writeRegister(PWR_MGMNT_1_REGISTER, CLOCK_SEL_PLL);
    writeRegister(PWR_MGMNT_2_REGISTER, SEN_ENABLE);
    writeRegister(CONFIG_REGISTER, GYRO_DLPF_184);
    writeRegister(SMPLRT_DIV_REGISTER, 0);

    // 1000DPS range
    setGyroRange(GYRO_RANGE_1000DPS);

    // Read calibration value from EEPROM
    calibration = (int16_t)(EEPROM.read(eepromAddress) | EEPROM.read(eepromAddress + 1) << 8);
}

void IMU::calibrate() {
    // Calibrate by averaging a number of readings. This determines the dps when the gyro is still

    for (int i = 0; i < IMU_CALIBRATION_COUNT; i++) {
        calibration += readRawGyroZ();
        delay(IMU_CALIBRATION_TIME);
    }

    calibration /= IMU_CALIBRATION_COUNT;

    // Save to EEPROM
    EEPROM.write(eepromAddress, calibration & 0xFF);
    EEPROM.write(eepromAddress + 1, (calibration >> 8) & 0xFF);
}

void IMU::setGyroRange(uint8_t range) {
    // Set the range on the gyro
    writeRegister(GYRO_CONFIG_REGISTER, range);

    // Determine the multiplier of the raw gyro data to convert to DPS
    double rangeMultiplier = 0;

    switch (range) {
    case GYRO_RANGE_250DPS:
        rangeMultiplier = 250.0;
        break;

    case GYRO_RANGE_500DPS:
        rangeMultiplier = 500.0;
        break;

    case GYRO_RANGE_1000DPS:
        rangeMultiplier = 1000.0;
        break;

    case GYRO_RANGE_2000DPS:
        rangeMultiplier = 2000.0;
        break;
    }

    gyroScale = rangeMultiplier / 32767.5;
}

void IMU::writeRegister(uint8_t address, uint8_t data) {
    // Write a value to a register
    spiClass->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    digitalWrite(csPin, LOW);
    spiClass->transfer(address);
    spiClass->transfer(data);
    digitalWrite(csPin, HIGH);
    spiClass->endTransaction();
}

void IMU::readRegisters(uint8_t address, uint8_t count, uint8_t *buffer) {
    // Read from a number of registers
    spiClass->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    digitalWrite(csPin, LOW);

    spiClass->transfer(address | SPI_READ);

    for (uint8_t i = 0; i < count; i++) {
        buffer[i] = spiClass->transfer(0);
    }

    digitalWrite(csPin, HIGH);
    spiClass->endTransaction();
}

int16_t IMU::readRawGyroZ() {
    // Get the raw z value
    uint8_t buffer[2];

    readRegisters(0x47, 2, buffer);

    return (int16_t)((buffer[0] << 8) | buffer[1]);
}

double IMU::readGyroZ() {
    // Convert raw z to dps taking into account calibration
    return (double)(readRawGyroZ() - calibration) * gyroScale;
}

void IMUFusion::init() {
    SPI2.begin();

    imu1.init();
    imu2.init();
    imu3.init();

    previousTime = micros();
}

void IMUFusion::calibrate() {
    // Calibrate all 3 gyros
    imu1.calibrate();
    imu2.calibrate();
    imu3.calibrate();
}

void IMUFusion::update() {
    // Read all 3 gyros
    double reading1 = imu1.readGyroZ();
    double reading2 = imu2.readGyroZ();
    double reading3 = imu3.readGyroZ();

    unsigned long currentTime = micros();

    // Determine the two gyros with closest DPS
    double diff12 = abs(reading1 - reading2);
    double diff23 = abs(reading2 - reading3);
    double diff31 = abs(reading3 - reading1);

    double smallestDiff = fmin(diff12, fmin(diff23, diff31));
    double finalDiff = 0;

    // Average the two smallest DPS values

    if (smallestDiff == diff12) {
        finalDiff = (reading1 + reading2) / 2.0;
    } else if (smallestDiff == diff23) {
        finalDiff = (reading2 + reading3) / 2.0;
    } else {
        finalDiff = (reading3 + reading1) / 2.0;
    }

    // Compute the heading using integration approximation
    heading += ((double)(currentTime - previousTime) / 1000000.0) * finalDiff;
    heading = doubleMod(heading, 360.0);

    previousTime = currentTime;
}

double IMUFusion::getHeading() {
    return heading;
}

void IMUFusion::resetHeading() {
    heading = 0;
}
