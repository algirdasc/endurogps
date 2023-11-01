#pragma once

#include <BluetoothSerial.h>
#include <esp_bt.h>

#include "EnduroGPS.h"
#include "Globals.h"
#include "GPS/Mode/BaseProxy.h"

class BluetoothProxy : public BaseProxy
{
    private:
        BluetoothSerial serialBT;        
        bool isDeviceConnected = false;

    public:        
        BluetoothProxy();

        String btSsid;

        void start();
        void stop();
        void handleProxy(uint8_t *data, size_t size);

        void setConnected(bool connected) { isDeviceConnected = connected; }
};
