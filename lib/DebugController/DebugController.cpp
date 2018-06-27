#include "DebugController.h"

void DebugController::init() {
    Serial5.begin(9600);
    Serial5.setTimeout(15);
}

void DebugController::send(String data, AppDataType dataCode) {
    // Send a value and prefix over UART / bluetooth
    Serial5.print("-" + String(dataCode) + ";" + data + "-");
}

void DebugController::appSendBallAngle(int angle) {
    send(String(angle), AppDataType::ball);
}

void DebugController::appSendBallStrength(int strength) {
    send(String(strength), AppDataType::ballStr);
}

void DebugController::appSendHeading(int heading) {
    send(String(heading), AppDataType::heading);
}

void DebugController::appSendString(String string) {
    send(string, AppDataType::info);
}

void DebugController::appSendLineSize(int size) {
    send(String(size), AppDataType::lineSize);
}

void DebugController::appSendLineAngle(int angle) {
    send(String(angle), AppDataType::lineAngle);
}
