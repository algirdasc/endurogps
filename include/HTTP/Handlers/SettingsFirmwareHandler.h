#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPUpdate.h>

#include "Translation.h"

class SettingsFirmwareHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == FIRMWARE_PAGE_BASE_URL;
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            if (requestMethod == HTTP_POST) {
                return handlePost(server, requestMethod, requestUri);
            }

            // TODO: check wifi mode for STA

            String htmlOutput = HTML::formStart();

            htmlOutput += HTML::formEnd(FIRMWARE_PAGE_SUBMIT);

            server.send(200, contentTypeHtml, 
                Template::generateBody(htmlOutput, FIRMWARE_PAGE_TITLE, FIRMWARE_PAGE_SUBTITLE)
            );

            WiFi.scanDelete();

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            WiFiClient client = server.client();

            httpUpdate.setLedPin(LED_BUILTIN);
            t_httpUpdate_return ret = httpUpdate.update(client, UPDATE_FILE_URL);

            switch (ret) {
                case HTTP_UPDATE_FAILED:
                    {
                        char errorMessage[128];
                        sprintf(errorMessage, "Error downloading update: (%d) %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
                        server.send(200, contentTypeHtml, 
                            Template::generateBody(errorMessage, FIRMWARE_PAGE_TITLE, FIRMWARE_PAGE_SUBTITLE)
                        );
                    }
                    break;
                case HTTP_UPDATE_NO_UPDATES:
                    server.send(200, contentTypeHtml,
                        Template::generateBody(FIRMWARE_PAGE_NEWEST_VERSION, FIRMWARE_PAGE_TITLE, FIRMWARE_PAGE_SUBTITLE)
                    );
                    break;
                case HTTP_UPDATE_OK:
                    server.send(200, contentTypeHtml,
                        Template::generateBody(FIRMWARE_PAGE_UPDATE_SUCCESS, FIRMWARE_PAGE_TITLE, FIRMWARE_PAGE_SUBTITLE)
                    );
                    break;
            }

            return true;
        }
};
