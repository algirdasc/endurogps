#pragma once

#include "WiFi.h"

class WifiMode 
{
    private:               
        void STA();
        void AP();
        void OFF();

    public:
        WifiMode();

        WiFiMode_t currentMode;
        bool fallbackToAP = false;
        String staSsid;
        String staPass;
        String apSsid;
        String apPass;

        void setMode(WiFiMode_t mode);
};
