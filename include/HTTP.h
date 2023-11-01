#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <FixedString.h>

#include "HTTP/HTTPCodes.h"
#include "HTTP/HTML.h"

class HTTP
{
public:
    static void redirect(WebServer &server)
    {
        redirect(server, server.header("Referer").c_str());
    }

    static void redirect(WebServer &server, const char *uri)
    {
        FixedString128 content;
        content.appendFormat(HTML_REDIRECT, uri, uri);

        server.sendHeader(F("Location"), uri);
        server.send(HTTP_CODE_REDIRECT, "text/html", content.c_str());
    }

    static void beginPage(WebServer &server, int code = HTTP_CODE_OK, const char *contentType = "text/html")
    {
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        server.send(code, contentType, HTML_HEADER);
    }

    static void endPage(WebServer &server)
    {
        server.sendContent(HTML::js());
        server.sendContent(HTML_FOOTER);

        server.sendContent("");
        server.client().stop();
    }

    static String escape(const char *userInput)
    {
        String escapedUserInput = userInput;

        escapedUserInput.replace("'", "\\'");
        escapedUserInput.replace("\"", "\\\"");

        return escapedUserInput;
    }
};
