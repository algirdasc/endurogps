#include <WiFi.h>
#include <WebServer.h>
#include "Params.h"

class SettingsWifiHandler : public RequestHandler
{
    private:
        Params params;

        String wifiModeValues[3] = {"WIFI_AP", "WIFI_STA", "WIFI_OFF"};
        String wifiModeLabels[3] = {"Access Point", "Client", "Off"};

        uint availableWifiStations = 0;
        String wifiStationsValues[128] = {};
        String wifiStationsLabels[128] = {};

    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/settings/wifi";
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            params.load();

            if (requestMethod == HTTP_POST) {
                return SettingsWifiHandler::handlePost(server, requestMethod, requestUri);
            }

            String htmlOutput = HTML::formStart();

            htmlOutput += HTML::select("Wireless mode", "wifiMode", wifiModeValues, wifiModeLabels, 3, params.wifiMode(params.storage.wifiMode));                                   

            htmlOutput += R"raw(<fieldset id="wifi_sta"><legend>Available networks <a href="/settings/wifi?scan=1" class="float-right">Scan</a></legend><div class="pure-g">)raw";

            htmlOutput += R"raw(<div class="pure-u-3-5">)raw";
            if (server.hasArg(("scan"))) {
                availableWifiStations = WiFi.scanNetworks();
                if (availableWifiStations > 0) {                               
                    for (int i = 0; i < availableWifiStations; i++) {
                        wifiStationsValues[i] = WiFi.SSID(i);
                        wifiStationsLabels[i] = WiFi.SSID(i) + (WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? " (Secured)" : "");
                    }            
                }

                WiFi.scanDelete();
               
                htmlOutput += HTML::select("Wireless network", "wifiStaSsid", wifiStationsValues, wifiStationsLabels, availableWifiStations, params.storage.wifiStaSsid);                
            } else {
                htmlOutput += HTML::input("Wireless network", "wifiStaSsid", params.storage.wifiStaSsid);
            }
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-2-5">)raw";
            htmlOutput += HTML::input("Password", "wifiStaPass", params.storage.wifiStaPass, "password");
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += HTML::formEnd();

            server.send(200, contentTypeHtml, Template::generateBody(htmlOutput, "Wireless"));

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {
            params.storage.wifiMode = params.wifiMode(server.arg("wifiMode"));

            if (server.hasArg("wifiStaSsid")) {
                params.storage.wifiStaSsid = server.arg("wifiStaSsid");
            }

            if (server.hasArg("wifiStaPass")) {
                params.storage.wifiStaPass = server.arg("wifiStaPass");
            }

            params.save();

            server.sendHeader("Location", "/settings/wifi", true);
            server.send(302, contentTypeHtml, "");

            return true;
        }
};
