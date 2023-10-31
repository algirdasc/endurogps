#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <FixedString.h>

#include "Params.h"
#include "HTTP.h"
#include "HTTP/HTML.h"
#include "HTTP/HTTPCodes.h"

const char WIFI_PAGE_BASE_URL[] = "/settings/wifi";

class SettingsWifiHandler : public RequestHandler
{
private:
    Params params;
    FixedString128 toast;

public:
    bool canHandle(HTTPMethod requestMethod, String requestUri)
    {
        return requestUri.equals(WIFI_PAGE_BASE_URL);
    }

    bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        params.load();

        if (requestMethod == HTTP_POST)
        {
            return SettingsWifiHandler::handlePost(server, requestMethod, requestUri);
        }

        server.sendContent(HTML_HEADER);
        
        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "WiFi Settings");

        server.sendContent(pageHeader.c_str());

        if (toast.length() > 0)
        {
            server.sendContent(toast.c_str());
            toast.clear();
        }

        server.sendContent(HTML_FORM_OPEN);
        server.sendContent(R"raw(<fieldset><legend>WiFi Mode settings</legend><div class="form-group">)raw");

        server.sendContent(HTML::select("Default WiFi mode", PARAM_WIFI_MODE));
        server.sendContent(HTML::option(PARAM_WIFI_MODE_AP, "Access Point", params.storage.wifiMode == WIFI_MODE_AP));
        server.sendContent(HTML::option(PARAM_WIFI_MODE_STA, "Client", params.storage.wifiMode == WIFI_MODE_STA));
        // TODO: enable when button will be implemented
        // server.sendContent(HTML::option(PARAM_WIFI_MODE_OFF, "Off", params.storage.wifiMode == WIFI_MODE_NULL));
        server.sendContent(HTML_SELECT_CLOSE);

        server.sendContent(R"raw(</div><div class="form-group">)raw");
        server.sendContent(HTML::checkbox("On client mode connection failure, fallback to AP mode", PARAM_WIFI_FALLBACK_AP, params.storage.wifiFallbackAp));

        server.sendContent(R"raw(</div></fieldset><fieldset><legend>Access Point Settings</legend><div class="row"><div class="col-sm-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("Access Point Name", PARAM_WIFI_AP_SSID, params.storage.wifiApSsid.c_str(), "text", 20, R"raw(pattern="[A-Za-z0-9\-_]+")raw"));
        server.sendContent(R"raw(</div></div><div class="col-sm-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("Access Point Password", PARAM_WIFI_AP_PASS, params.storage.wifiApPass.c_str(), "password"));
        server.sendContent(R"raw(</div></div></div></fieldset><fieldset><legend>Client Settings</legend><div class="row"><div class="col-sm-5 col-xs-9"><div class="form-group">)raw");

        int availableWifiStations = -1;
        if (server.hasArg(F("scan")))
        {
            availableWifiStations = 0; //WiFi.scanNetworks();
            if (availableWifiStations > 0)
            {
                server.sendContent(HTML::select("Client Network Name", PARAM_WIFI_STA_SSID));
                for (int i = 0; i < availableWifiStations; i++)
                {
                    server.sendContent(HTML::option(WiFi.SSID(i).c_str(), WiFi.SSID(i).c_str(), WiFi.SSID(i).equals(params.storage.wifiStaSsid)));
                }                
                server.sendContent(HTML_SELECT_CLOSE);
            } else {
                server.sendContent(R"raw(<div class="text-danger">No wireless networks found</div>)raw");
            }

            WiFi.scanDelete();
        }

        if (availableWifiStations <= 0) {
            server.sendContent(HTML::input("Client Network Name", PARAM_WIFI_STA_SSID, params.storage.wifiStaSsid.c_str()));
        }

        server.sendContent(R"raw(</div></div><div class="col-sm-1 col-xs-3"><div class="form-group"><label>&nbsp;</label><a class="btn btn-info block" href="/settings/wifi?scan=1">Scan</a></div></div><div class="col-sm-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("Client Network Password", PARAM_WIFI_STA_PASS, params.storage.wifiStaPass.c_str(), "password"));
        server.sendContent("</div></div></div></fieldset>");

        server.sendContent(HTML_FORM_CLOSE);
        server.sendContent(HTML::js());
        server.sendContent(HTML_FOOTER);

        return true;
    }

    bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        params.storage.wifiMode = params.wifiMode(server.arg(PARAM_WIFI_MODE));
        params.storage.wifiFallbackAp = server.hasArg(PARAM_WIFI_FALLBACK_AP);
        params.storage.wifiApSsid = HTTP::escape(server.arg(PARAM_WIFI_AP_SSID).c_str());
        params.storage.wifiApPass = HTTP::escape(server.arg(PARAM_WIFI_AP_PASS).c_str());
        params.storage.wifiStaSsid = HTTP::escape(server.arg(PARAM_WIFI_STA_SSID).c_str());
        params.storage.wifiStaPass = HTTP::escape(server.arg(PARAM_WIFI_STA_PASS).c_str());

        params.save();

        toast.append(HTML_SETTINGS_SUCCESS);

        HTTP::redirect(server, WIFI_PAGE_BASE_URL);

        return true;
    }
};
