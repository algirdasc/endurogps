#include <WiFi.h>
#include <WebServer.h>
#include "Settings.h"

Settings settings;

class SettingsWifiHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/settings/wifi";
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            if (requestMethod == HTTP_POST) {
                return SettingsWifiHandler::handlePost(server, requestMethod, requestUri);
            }

            String htmlOutput = ""
                "<form method='POST'>"
                "<select name='wifiMode'>"
                "<option value='WIFI_AP'>Access Point</option>"
                "<option value='WIFI_STA'>Client</option>"
                "<option value='WIFI_OFF'>Off</option>"
                "</select>"
                "<select name='wifiStaSsid'>";

            int networks = WiFi.scanNetworks();
            if (networks > 0) {                               
                for (int i = 0; i < networks; i++) {
                    htmlOutput += "<option value='";
                    htmlOutput += WiFi.SSID(i).c_str();
                    htmlOutput += "'>";
                    htmlOutput += WiFi.SSID(i).c_str();
                    htmlOutput += "</option>";
                }            
            }
            
            htmlOutput += "</select>";
            htmlOutput += "<input type='password' name='wifiStaPass' />";
            htmlOutput += "<input type='submit' />";
            htmlOutput += "</form>";

            server.send(200, "text/html", htmlOutput);

            WiFi.scanDelete();

            return true;
        }

        bool handlePost(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            if (server.hasArg("wifiMode")) {
                settings.storage.wifiMode = WIFI_STA;
            }

            if (server.hasArg("wifiStaSsid")) {
                settings.storage.wifiStaSsid = server.arg("wifiStaSsid");
            }

            if (server.hasArg("wifiStaPass")) {
                settings.storage.wifiStaSsid = server.arg("wifiStaPass");
            }
            
            settings.save();

            server.send(200, "text/html", "Restarting...");

            ESP.restart();

            return true;
        }
};
