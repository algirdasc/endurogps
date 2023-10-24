#pragma once

#include "WiFi.h"

class WifiMode 
{
    private:        
        char accessPointSSID[20];
        
        void STA();
        void AP();
        void OFF();

    public:
        WifiMode();

        WiFiMode_t currentMode;
        bool fallbackToAP = false;
        String staSsid;
        String staPass;

        void setMode(WiFiMode_t mode);
};
