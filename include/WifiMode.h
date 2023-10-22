#pragma once

#include "WiFi.h"

#define MAX_AP_NAME_SIZE 20

class WifiMode 
{
    private:        
        char accessPointSSID[MAX_AP_NAME_SIZE];
        String STASsid;
        String STAKey;

        void STA();
        void AP();
        void OFF();

    public:
        WifiMode();

        WiFiMode_t currentMode;
        bool isConnected = false;

        void mode(WiFiMode_t mode);
        void setSTAcredentials(String STASsid, String STAKey);
};
