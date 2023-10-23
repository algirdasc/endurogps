#pragma once

#include <WiFi.h>
#include <WebServer.h>

#include "Params.h"
#include "HTTP/HTTPCodes.h"


const char WIFI_PAGE_BASE_URL[] PROGMEM = "/settings/wifi";
const char WIFI_PAGE_WIFI_MODE[] PROGMEM = "Wireless mode";
const char WIFI_PAGE_WIFI_SSID[] PROGMEM = "Wireless network";

const char *WIFI_PAGE_MODE_VALUES[] PROGMEM = {PARAM_WIFI_AP, PARAM_WIFI_STA, PARAM_WIFI_OFF};
const char *WIFI_PAGE_MODE_LABELS[] PROGMEM = {"Access Point", "Client", "Off"};


class SettingsWifiHandler : public RequestHandler
{
    private:
        Params params;

        uint availableWifiStations = 0;
        String wifiStationsValues[128] = {};
        String wifiStationsLabels[128] = {};

    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {            
            return requestUri == WIFI_PAGE_BASE_URL;
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            params.load();

            if (requestMethod == HTTP_POST) {
                return SettingsWifiHandler::handlePost(server, requestMethod, requestUri);
            }

            String htmlOutput = HTML::formStart();

            htmlOutput += HTML::select(WIFI_PAGE_WIFI_MODE, PARAM_WIFI_MODE, WIFI_PAGE_MODE_VALUES, WIFI_PAGE_MODE_LABELS, 3, params.wifiMode(params.storage.wifiMode));
            htmlOutput += F(R"raw(<fieldset id="wifi_sta"><legend>Available networks <a href="/settings/wifi?scan=1" class="float-right">Scan</a></legend><div class="pure-g"><div class="pure-u-3-5">)raw");

            if (server.hasArg(F("scan"))) {
                availableWifiStations = WiFi.scanNetworks();
                if (availableWifiStations > 0) {                               
                    for (int i = 0; i < availableWifiStations; i++) {
                        wifiStationsValues[i] = WiFi.SSID(i);
                        wifiStationsLabels[i] = WiFi.SSID(i) + (WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? " (Secured)" : "");
                    }            
                }

                WiFi.scanDelete();
               
                //TODO: fix
                // htmlOutput += HTML::select(WIFI_PAGE_WIFI_SSID, PARAM_WIFI_STA_SSID, wifiStationsValues, wifiStationsLabels, availableWifiStations, params.storage.wifiStaSsid);                
            } else {
                htmlOutput += HTML::input(WIFI_PAGE_WIFI_SSID, PARAM_WIFI_STA_SSID, params.storage.wifiStaSsid);
            }

            htmlOutput += F(R"raw(</div><div class="pure-u-2-5">)raw");
            htmlOutput += HTML::input(F("Password"), PARAM_WIFI_STA_PASS, params.storage.wifiStaPass, F("password"));
            htmlOutput += F("</div></div></fieldset>");

            htmlOutput += HTML::formEnd();

            server.send(HTTP_CODE_OK, contentTypeHtml, 
                Template::generateBody(htmlOutput, F("Wireless"))
            );

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            params.storage.wifiMode = params.wifiMode(server.arg(PARAM_WIFI_MODE));

            if (server.hasArg(PARAM_WIFI_STA_SSID)) {
                params.storage.wifiStaSsid = server.arg(PARAM_WIFI_STA_SSID);
            }

            if (server.hasArg(PARAM_WIFI_STA_PASS)) {
                params.storage.wifiStaPass = server.arg(PARAM_WIFI_STA_PASS);
            }

            params.save();

            Template::redirect(server, WIFI_PAGE_BASE_URL);

            return true;
        }
};
