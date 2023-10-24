#pragma once

#include <WebServer.h>

#include "HTTP/Template.h"
#include "HTTP/HTTPCodes.h"
#include "HTTP/Assets/HTML/Index.h"

class IndexHandler : public RequestHandler
{   
    private:
        const int bToMb = 1048576;
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/";
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            char buff[1024];
            sprintf(buff, HTML_INDEX, 
                // WIFI
                "-", "-",
                // Memory
                ESP.getCpuFreqMHz(), ESP.getHeapSize(), ESP.getFreeHeap(), ESP.getPsramSize(), ESP.getFreePsram() 
            );

            server.send(HTTP_CODE_OK, contentTypeHtml, 
                Template::generateBody(buff)
            );

            return true;
        }
};
