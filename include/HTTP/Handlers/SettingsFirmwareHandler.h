#include <WiFi.h>
#include <WebServer.h>

class SettingsFirmwareHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/settings/firmware";
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            String htmlOutput = "";

            server.send(200, contentTypeHtml, Template::generateBody(htmlOutput));

            WiFi.scanDelete();

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            return true;
        }
};
