#include <Arduino.h>
#include <WebServer.h>

typedef std::function<void(void)> THandlerFunction;
class HTTP
{
    private:
        uint port;
    
    public:
        WebServer server;

        HTTP(uint port);
        
        // Control
        void start();
        void stop();
        void handleClient();
        void redirect(String uri);

        // Handles
        void notFound();
        void poweredOff();       

        // Mirror functions.        
        void on(const Uri &uri, THandlerFunction fn);
};
