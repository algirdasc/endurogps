#pragma once

#include <WebServer.h>

#include "HTTP/Template.h"
#include "HTTP/HTTPCodes.h"

const char INDEX_PAGE_TEMPLATE[] PROGMEM = 
    "<a href='/settings'>Settings</a>\n"
    "<a href='/sd/browse'>View files</a>\n";    

class IndexHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/";
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            server.send(HTTP_CODE_OK, contentTypeHtml, Template::generateBody(INDEX_PAGE_TEMPLATE));

            return true;
        }
};
