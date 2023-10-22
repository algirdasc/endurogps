#pragma once

#include <WiFi.h>
#include "EnduroGPS.h"

class NMEAServer
{
    private:
        WiFiServer server;
        WiFiClient client;

    public:
        NMEAServer();

        bool isStarted = false;

        void start();
        void stop();
        void handleClient();        
};
