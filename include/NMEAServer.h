#include <WiFi.h>
#include "EnduroGPS.h"

class NMEAServer
{
    private:
        uint port;
        WiFiServer server;
        WiFiClient client;

    public:
        NMEAServer(uint port);

        bool isStarted = false;

        void start();
        void stop();
        void handleClient();        
};
