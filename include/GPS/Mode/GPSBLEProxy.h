#pragma once

#include "BluetoothSerial.h"
#include "esp_bt.h"

class GPSBLEProxy
{
    private:
        BluetoothSerial serialBT;
        ulong baudRate;
        bool deviceConnected = false;

        void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

    public:
        GPSBLEProxy();

        void start();
        bool stop();        
};
