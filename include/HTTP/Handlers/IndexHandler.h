#pragma once

#include <WebServer.h>

#include "HTTP/HTTPCodes.h"
#include "HTTP/Assets/HTML/Index.h"

class IndexHandler : public RequestHandler
{
public:
    bool canHandle(HTTPMethod requestMethod, String requestUri)
    {
        return requestUri.equals("/");
    }

    bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        // FixedString<1024> body;
        // body.appendFormat("%s", "Something");
        // body.debug();
        
        // server.send(HTTP_CODE_OK, contentTypeHtml, Template::render(body.c_str()));

        return true;
    }
};
