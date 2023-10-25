#pragma once

#include <WebServer.h>
#include "HTTP/Template.h"
#include "HTTP/HTTPCodes.h"
#include "HTTP/Assets/CSS/Style.h"
// #include "HTTP/Assets/JS/Script.h"

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

            if (requestUri == "/css/style.css") {
                server.send_P(HTTP_CODE_OK, contentTypeCss, CSS_STYLE);
            }

            // if (requestUri == "/js/script.css") {
            //     server.send_P(HTTP_CODE_OK, contentTypeCss, JS_SCRIPT);
            // }

            return true;
        }
};
