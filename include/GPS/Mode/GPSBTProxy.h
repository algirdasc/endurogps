#pragma once

#include <BluetoothSerial.h>
#include <esp_bt.h>

#include "EnduroGPS.h"
#include "GPS/Mode/BaseProxy.h"

class GPSBTProxy : public BaseProxy
{
    private:
        BluetoothSerial serialBT;
        char deviceID[20];
        bool isDeviceConnected = false;

    public:
        GPSBTProxy();

        void start();
        void stop();
        void handle(uint8_t *data, size_t size);

        void setConnected(bool connected) { isDeviceConnected = connected; }
};
