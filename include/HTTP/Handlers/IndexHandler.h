#include <WebServer.h>

const char PAGE_TEMPLATE[] PROGMEM = 
    "<a href='/settings'>Settings</a>\n"
    "<a href='/sd/browse'>View flies</a>\n";

class IndexHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && requestUri == "/";
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.send_P(200, "text/html", PAGE_TEMPLATE);

            return true;
        }
};
