#pragma once

#include <WebServer.h>

#include "HTTP/HTTPCodes.h"

class IndexHandler : public RequestHandler
{
public:
    bool canHandle(HTTPMethod requestMethod, String requestUri)
    {
        return requestUri.equals("/");
    }

    bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        Battery battery;

        HTTP::beginPage(server);

        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "Device Status");

        server.sendContent(pageHeader.c_str());

        server.sendContent(R"raw(<div class="row"><div class="col-md-6 col-md-offset-3"><h3 class="text-center">WiFi Settings</h3><dl class="dl-horizontal">)raw");

        FixedString128 wSsid;
        wSsid.appendFormat(R"raw(<dt>SSID: </dt><dd>%s</dd>)raw", WiFi.SSID());
        server.sendContent(wSsid.c_str());

        FixedString128 wIpAddr;
        wIpAddr.appendFormat(R"raw(<dt>IP: </dt><dd>%s</dd>)raw", WiFi.localIP().toString());
        server.sendContent(wIpAddr.c_str());

        server.sendContent(R"raw(</dl><h3 class="text-center">Battery:</h3><dl class="dl-horizontal">)raw");

        FixedString128 bPercentage;
        bPercentage.appendFormat(R"raw(<dt>Percentage:</dt><dd>%d%%</dd>)raw", battery.percentage());

        FixedString128 bVoltage;
        bVoltage.appendFormat(R"raw(<dt>Voltage:</dt><dd>%.2f V</dd>)raw", battery.voltage());

        server.sendContent(bPercentage.c_str());
        server.sendContent(bVoltage.c_str());
        server.sendContent(R"raw(</dl><h3 class="text-center">Core:</h3><dl class="dl-horizontal">)raw");

        FixedString128 cFreq;
        cFreq.appendFormat(R"raw(<dt>Chip:</dt><dd> %s (Rev.: %d) %d Cores, %d MHz</dd>)raw", ESP.getChipModel(), ESP.getChipRevision(), ESP.getChipCores(), ESP.getCpuFreqMHz());

        FixedString128 cHeap;
        cHeap.appendFormat(R"raw(<dt>Heap:</dt><dd>%d KB Free / %d KB Total</dd>)raw", ESP.getFreeHeap() / 1024, ESP.getHeapSize() / 1024);

        FixedString128 cPsram;
        cPsram.appendFormat(R"raw(<dt>PSRAM:</dt><dd>%d KB Free / %d KB Total</dd>)raw", ESP.getFreePsram() / 1024, ESP.getPsramSize() / 1024);

        FixedString128 cSdk;
        cSdk.appendFormat(R"raw(<dt>SDK Ver:</dt><dd>%s</dd>)raw", ESP.getSdkVersion());

        FixedString128 cCompiled;
        cCompiled.appendFormat(R"raw(<dt>Build date: </dt><dd>%s</dd>)raw", __DATE__);

        server.sendContent(cFreq.c_str());
        server.sendContent(cHeap.c_str());
        server.sendContent(cPsram.c_str());
        server.sendContent(cSdk.c_str());
        server.sendContent(cCompiled.c_str());

        server.sendContent(R"raw(</dl></div></div>)raw");

        HTTP::endPage(server);

        return true;
    }
};
