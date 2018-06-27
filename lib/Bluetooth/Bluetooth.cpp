#include "Bluetooth.h"

void Bluetooth::init() {
    // Begin UART
    Serial5.begin(BLUETOOTH_BAUD);    
}

void Bluetooth::update(BluetoothData data) {
    // Send and recieve

    thisData = data;
    
    send();
    recieve();
}

void Bluetooth::send() {
    // Sends a start of packet byte (10000000) and then all other data split into 7 bit sections

    Serial5.write(BLUETOOTH_START_BYTE);

    Serial5.write((thisData.ballData.angle >> 7) & 0x7F);
    Serial5.write(thisData.ballData.angle & 0x7F);

    Serial5.write((thisData.ballData.strength >> 7) & 0x7F);
    Serial5.write(thisData.ballData.strength & 0x7F);
    
    Serial5.write((thisData.heading >> 7) & 0x7F);
    Serial5.write(thisData.heading & 0x7F);

    Serial5.write(thisData.ballIsOut);

    Serial5.write(thisData.playMode);

    Serial5.write(thisData.isOnField);

    Serial5.write(((int8_t)thisData.robotPosition.x >> 7) & 0x7F);
    Serial5.write((int8_t)thisData.robotPosition.x & 0x7F);

    Serial5.write(((int8_t)thisData.robotPosition.y >> 7) & 0x7F);
    Serial5.write((int8_t)thisData.robotPosition.y & 0x7F);
}

void Bluetooth::recieve() {
    // Receives the data from the other robot

    bool nothingRecieved = true;

    // Loops while the amount of data is >= the packet size
    while (Serial5.available() >= BLUETOOTH_PACKET_SIZE) {
        uint8_t first = Serial5.read();

        // Makes sure the first byte is the start of packet indicator, otherwise keeps looping
        if (first == BLUETOOTH_START_BYTE) {
            uint8_t dataBuffer[BLUETOOTH_PACKET_SIZE - 1];

            // Put all the data into an array
            for (int i = 0; i < BLUETOOTH_PACKET_SIZE - 1; i++) {
                dataBuffer[i] = Serial5.read();
            }

            otherData.ballData = BallData((dataBuffer[0] << 7) | dataBuffer[1], (dataBuffer[2] << 7) | dataBuffer[3]);
            otherData.heading = (dataBuffer[4] << 7) | dataBuffer[5];
            otherData.ballIsOut = (bool)dataBuffer[6];
            otherData.playMode = static_cast<PlayMode>(dataBuffer[7]);
            otherData.isOnField = (bool)dataBuffer[8];
            otherData.robotPosition = Point((int8_t)((dataBuffer[9] << 7) | dataBuffer[10]), (int8_t)((dataBuffer[11] << 7) | dataBuffer[12]));

            nothingRecieved = false;
            connectedTimer.update();
        }
    }

    // Update the connected timer
    isConnected = !nothingRecieved || !connectedTimer.timeHasPassedNoUpdate();

    if (isConnected) {
        if (!previouslyConnected) {
            previouslyConnected = true;
        }
    } else {
        otherData = BluetoothData();
    }
}

void Bluetooth::disconnect() {
    isConnected = false;
}