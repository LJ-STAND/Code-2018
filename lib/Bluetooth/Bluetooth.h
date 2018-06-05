#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <Config.h>
#include <PlayMode.h>
#include <Timer.h>
#include <BluetoothData.h>

class Bluetooth {
public:
    void init();
    void update(BluetoothData data);
    void disconnect();

    BluetoothData otherData = BluetoothData();
    bool isConnected = false;
    bool previouslyConnected = false;

private:
    BluetoothData thisData = BluetoothData();

    Timer connectedTimer = Timer(BLUETOOTH_LOST_COMMUNICATION_TIME);
    
    void send();
    void recieve();
};

#endif // BLUETOOTH_H