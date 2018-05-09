#include "DebugController.h"

void DebugController::init() {}

// void DebugController::appSendIMU(double angle) {
//     Bluetooth::send(angle, BluetoothDataType::compass);
// }

void DebugController::appSendTSOPs(double angle) {
    Bluetooth::send(angle, BluetoothDataType::tsop);
}

void DebugController::appSendString(String string) {
    Bluetooth::send(string);
}

void DebugController::appSendLightSensors(uint16_t first16Bit, uint16_t second16Bit) {
    Bluetooth::send(String(first16Bit | (second16Bit << 16)), BluetoothDataType::lightSensor);
}

void DebugController::appSendOrbitAngle(double angle) {
    Bluetooth::send(String(angle), BluetoothDataType::orbitAngle);
}

// void DebugController::appSendPixy(double x, double y, double width, double height) {
//     String sendString = String(x) + "," + String(y) + "," + String(width) + "," + String(height);
//     Bluetooth::send(sendString, BluetoothDataType::goal);
// }

void DebugController::appSendRobotPosition(double lineAngle, double lineSize) {
    String sendString = String(lineAngle) + "," + String(lineSize);
    Bluetooth::send(sendString, BluetoothDataType::btRobotPosition);
}
