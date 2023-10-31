#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <FixedString.h>

#include "HTTP/HTTPCodes.h"
#include "HTTP/HTML.h"

class HTTP
{
public:
    static void redirect(WebServer &server, const char *uri)
    {
        FixedString128 content;
        content.appendFormat(HTML_REDIRECT, uri, uri);

        server.sendHeader(F("Location"), uri);
        server.send(HTTP_CODE_REDIRECT, "text/html", content.c_str());
    }

    static String escape(const char *userInput)
    {
        String escapedUserInput = userInput;

        escapedUserInput.replace("'", "\\'");
        escapedUserInput.replace("\"", "\\\"");

        return escapedUserInput;
    }
};
