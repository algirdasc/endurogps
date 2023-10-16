#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

class HTTP
{
    private:
        int port;
        WebServer server;
    
    public:
        HTTP(int port);
        
        // Control
        void start();
        void stop();
        void handleClient();
      
        // Handles
        void notFound();
};
