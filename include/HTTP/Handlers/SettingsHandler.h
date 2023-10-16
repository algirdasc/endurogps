#include <WebServer.h>

class SettingsHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && requestUri.startsWith("/settings");
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.send(200, "text/plain", "OK! settings");

            return true;
        }
};