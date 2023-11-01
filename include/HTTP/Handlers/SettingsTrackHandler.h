#pragma once

#include <WebServer.h>

#include "Params.h"
#include "HTTP.h"
#include "HTTP/HTML.h"
#include "HTTP/HTTPCodes.h"

const char TRACK_PAGE_BASE_URL[] = "/settings/track";

class SettingsTrackHandler : public RequestHandler
{
private:
    Params params;
    FixedString128 toast;

public:
    bool canHandle(HTTPMethod requestMethod, String requestUri)
    {
        return requestUri.equals(TRACK_PAGE_BASE_URL);
    }

    bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        if (requestMethod == HTTP_POST)
        {
            return handlePost(server, requestMethod, requestUri);
        }

        params.load();

        HTTP::beginPage(server);

        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "Track Settings");
        server.sendContent(pageHeader.c_str());

        if (toast.length() > 0)
        {
            server.sendContent(toast.c_str());
            toast.clear();
        }

        server.sendContent(HTML_FORM_OPEN);

        server.sendContent(R"raw(<fieldset><legend>Start / Finish Line</legend><div class="row">)raw");

        server.sendContent(R"raw(<div class="col-sm-6"><div class="row"><div class="col-xs-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("L1 Latitude", PARAM_TRACK_L1_LATITUDE, params.storage.l1Lat.c_str(), "text", 12, R"raw(pattern="[0-9.]+")raw"));
        server.sendContent(R"raw(</div></div><div class="col-xs-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("L1 Longitude", PARAM_TRACK_L1_LONGITUDE, params.storage.l1Lon.c_str(), "text", 12, R"raw(pattern="[0-9.]+")raw"));
        server.sendContent(R"raw(</div></div></div></div>)raw");

        server.sendContent(HTML::checkbox("Use current position", "l1Current", true));

        server.sendContent(R"raw(<div class="col-sm-6"><div class="row"><div class="col-xs-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("L2 Latitude", PARAM_TRACK_L2_LATITUDE, params.storage.l2Lat.c_str(), "text", 12, R"raw(pattern="[0-9.]+")raw"));
        server.sendContent(R"raw(</div></div><div class="col-xs-6"><div class="form-group">)raw");
        server.sendContent(HTML::input("L2 Longitude", PARAM_TRACK_L2_LONGITUDE, params.storage.l2Lon.c_str(), "text", 12, R"raw(pattern="[0-9.]+")raw"));
        server.sendContent(R"raw(</div></div></div></div></fieldset>)raw");

        server.sendContent(HTML_FORM_CLOSE);

        HTTP::endPage(server);

        return true;
    }

    bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        params.storage.l1Lat = HTTP::escape(server.arg(PARAM_TRACK_L1_LATITUDE).c_str());
        params.storage.l1Lon = HTTP::escape(server.arg(PARAM_TRACK_L1_LONGITUDE).c_str());
        params.storage.l2Lat = HTTP::escape(server.arg(PARAM_TRACK_L2_LATITUDE).c_str());
        params.storage.l2Lon = HTTP::escape(server.arg(PARAM_TRACK_L2_LONGITUDE).c_str());

        params.save();

        toast.append(HTML_SETTINGS_SUCCESS);

        HTTP::redirect(server, TRACK_PAGE_BASE_URL);

        return true;
    }
};
