#include "NMEAServer.h"

NMEAServer::NMEAServer(uint port)
{
    NMEAServer::port = port;    
    WiFiClient client;
}

void NMEAServer::start()
{
    WiFiServer server(NMEAServer::port);

    server.begin();
    server.setNoDelay(true);
    
    isStarted = true;
    log_d("Starting NMEA TCP Server on port TCP %d", port);
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
        return;
    }

    if (!server.hasClient()) {
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
