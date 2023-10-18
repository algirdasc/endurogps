#ifndef WIFIMODE_H
#define WIFIMODE_H

#include "WiFi.h"

#define MAX_AP_NAME_SIZE 20

class WifiMode 
{
    private:
        char accessPointSSID[MAX_AP_NAME_SIZE];

    public:
        WifiMode();

        bool isConnected = false;

        void start(WiFiMode_t mode);

        void STA();
        void AP();
        void OFF();
};

#endif
