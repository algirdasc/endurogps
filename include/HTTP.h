#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "EnduroGPS.h"

typedef std::function<void(void)> THandlerFunction;
class HTTP
{
    public:
        WebServer server;

        HTTP();
        
        // Control
        void start();
        void stop();
        void handleClient();
        void redirect(String uri);

        // Handles
        void notFound();
        void pleaseWait();       

        // Mirror functions.        
        void on(const Uri &uri, THandlerFunction fn);
};
