#include "Bluetooth.h"

void Bluetooth::init() {
    Serial5.begin(BLUETOOTH_BAUD);    
}

void Bluetooth::update(BluetoothData data) {
    thisData = data;
    
    send();
    recieve();
}

void Bluetooth::send() {
    Serial5.write(BLUETOOTH_START_BYTE);
    Serial5.write((thisData.ballAngle >> 7) & 0x7F);
    Serial5.write(thisData.ballAngle & 0x7F);
    Serial5.write(thisData.ballStrength & 0x7F);
    Serial5.write((thisData.heading >> 7) & 0x7F);
    Serial5.write(thisData.heading & 0x7F);
    Serial5.write(thisData.ballIsOut);
    Serial5.write(thisData.playMode);
    Serial5.write(thisData.isOnField);
}

void Bluetooth::recieve() {
    bool nothingRecieved = true;

    while (Serial5.available() >= BLUETOOTH_PACKET_SIZE) {
        uint8_t first = Serial5.read();

        if (first == BLUETOOTH_START_BYTE) {
            uint8_t dataBuffer[BLUETOOTH_PACKET_SIZE - 1];

            for (int i = 0; i < CAMERA_PACKET_SIZE - 1; i++) {
                dataBuffer[i] = Serial3.read();
            }

            otherData.ballAngle = (dataBuffer[0] << 7) | dataBuffer[1];
            otherData.ballStrength = dataBuffer[2];
            otherData.heading = (dataBuffer[3] << 7) | dataBuffer[4];
            otherData.ballIsOut = (bool)dataBuffer[5];
            otherData.playMode = static_cast<PlayMode>(dataBuffer[6]);
            otherData.isOnField = (bool)dataBuffer[6];

            nothingRecieved = false;
            connectedTimer.update();
        }
    }

    isConnected = !nothingRecieved || !connectedTimer.timeHasPassedNoUpdate();

    if (isConnected) {
        if (!previouslyConnected) {
            previouslyConnected = true;
        }
    } else {
        otherData = BluetoothData();
    }
}
