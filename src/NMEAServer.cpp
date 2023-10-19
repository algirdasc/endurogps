#include "NMEAServer.h"

NMEAServer::NMEAServer()
{
    WiFiServer server(NMEA_PORT);
}

void NMEAServer::start()
{    
    server.begin();
    server.setNoDelay(true);

    isStarted = true;
    log_d("Starting NMEA TCP Server on port TCP %d", NMEA_PORT);
}

void NMEAServer::stop()
{
    server.stop();
    isStarted = false;
    log_d("Stoping NMEA TCP Server");
}

void NMEAServer::handleClient()
{
    if (!isStarted) {
        log_d("Not started");
        
        return;
    }

    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    
    if (client.connected()) {
        server.available().stop();
        log_w("NMEA TCP Connection rejected");
    } else {
        client = server.available();
        log_d("NMEA TCP Connection accepted from client: %s", client.remoteIP().toString().c_str());
    }
}
