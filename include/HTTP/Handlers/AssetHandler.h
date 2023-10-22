#pragma once

#include <WebServer.h>
#include "HTTP/Template.h"
#include "HTTP/Assets/Style.h"
#include "HTTP/Assets/PureCSS.h"

class AssetHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && (requestUri.endsWith(".css") || requestUri.endsWith(".js"));
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            server.sendHeader("Cache-Control", "max-age=86400");

            if (requestUri == "/pure-css.css") {
                server.send_P(200, contentTypeCss, PURE_CSS);
            }

            if (requestUri == "/style.css") {
                server.send_P(200, contentTypeCss, STYLE_CSS);
            }

            return true;
        }
};
