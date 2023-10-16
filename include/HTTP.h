#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriBraces.h>



class HTTP
{
    private:
        int port;
        WebServer web;
    
    public:
        HTTP(int port);
        
        // Control
        void start();
        void stop();
        void handleClient();

        // Helpers
        String generateHeader(bool addMenu);
        String generateBody(String body, bool addMenu);
        
        // Handles
        void notFound();
};
