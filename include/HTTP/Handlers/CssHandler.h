#include <WebServer.h>
#include "HTTP/Template.h"
#include "HTTP/Style.h"

class CssHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && requestUri == "/style.css";
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.sendHeader("Cache-Control", "max-age=86400");
            server.send_P(200, contentTypeCss, STYLE_CSS);   

            return true;
        }
};
