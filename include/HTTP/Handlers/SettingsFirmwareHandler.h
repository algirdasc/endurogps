#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPUpdate.h>

#include "HTTP/HTTPCodes.h"

const char FIRMWARE_PAGE_BASE_URL[] PROGMEM = "/settings/firmware";
const char FIRMWARE_PAGE_TITLE[] PROGMEM = "Firmware update";
const char FIRMWARE_PAGE_SUBTITLE[] PROGMEM = "Current version: " FIRMWARE;
const char FIRMWARE_PAGE_SUBMIT[] PROGMEM = "Update to latest version";
const char FIRMWARE_PAGE_NEWEST_VERSION[] PROGMEM = "You have newest firmware version";
const char FIRMWARE_PAGE_UPDATE_SUCCESS[] PROGMEM = "Update was successful!";
const char FIRMWARE_PAGE_UPDATE_FAILED[] PROGMEM = "Error downloading update: (%d) %s";


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

            server.send(HTTP_CODE_OK, contentTypeHtml, 
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

            char message[128];
            switch (ret) {
                case HTTP_UPDATE_FAILED:
                    sprintf(message, FIRMWARE_PAGE_UPDATE_FAILED, httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
                    break;
                case HTTP_UPDATE_NO_UPDATES:
                    sprintf(message, FIRMWARE_PAGE_NEWEST_VERSION);
                    break;
                case HTTP_UPDATE_OK:
                    sprintf(message, FIRMWARE_PAGE_UPDATE_SUCCESS);
                    break;
            }

            server.send(HTTP_CODE_OK, contentTypeHtml,
                Template::generateBody(message, FIRMWARE_PAGE_TITLE, FIRMWARE_PAGE_SUBTITLE)
            );

            return true;
        }
};
