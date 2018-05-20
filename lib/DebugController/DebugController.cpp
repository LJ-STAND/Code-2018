#include "DebugController.h"

void DebugController::init() {
    Serial5.begin(9600);
    Serial5.setTimeout(15);
}

void DebugController::send(String data, int dataCode) {
    Serial.print("-" + String(dataCode) + ";" + data + "-");
    Serial5.print("-" + String(dataCode) + ";" + data + "-");
}

void DebugController::appSendBallAngle(int angle) {
    send(String(angle), BluetoothDataType::ball);
}

void DebugController::appSendBallStrength(int strength) {
    send(String(strength), BluetoothDataType::ballStr);
}

void DebugController::appSendHeading(int heading) {
    send(String(heading), BluetoothDataType::heading);
}

void DebugController::appSendString(String string) {
    send(string, BluetoothDataType::info);
}

void DebugController::appSendLineSize(int size) {
    send(String(size), BluetoothDataType::lineSize);
}

void DebugController::appSendLineAngle(int angle) {
    send(String(angle), BluetoothDataType::lineAngle);
}
