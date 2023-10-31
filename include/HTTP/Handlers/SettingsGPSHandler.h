#pragma once

#include <WebServer.h>

#include "Params.h"
#include "GPS/GPSPort.h"
#include "HTTP.h"
#include "HTTP/HTML.h"
#include "HTTP/HTTPCodes.h"

const char GPS_PAGE_BASE_URL[] = "/settings/gps";

class SettingsGPSHandler : public RequestHandler
{
private:
    Params params;
    FixedString128 toast;

public:
    bool canHandle(HTTPMethod requestMethod, String requestUri)
    {
        return requestUri.equals(GPS_PAGE_BASE_URL);
    }

    bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        params.load();

        if (requestMethod == HTTP_POST)
        {
            return handlePost(server, requestMethod, requestUri);
        }

        HTTP::beginPage(server);
        
        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "GPS Settings");
        server.sendContent(pageHeader.c_str());

        if (toast.length() > 0)
        {
            server.sendContent(toast.c_str());
            toast.clear();
        }

        server.sendContent(HTML_FORM_OPEN);

        server.sendContent(R"raw(<fieldset><legend>Logging</legend><div class="row"><div class="col-sm-6"><div class="form-group">)raw");

        server.sendContent(HTML::select("GPS Mode", PARAM_GPS_MODE));
        server.sendContent(HTML::option(GPS_MODE_SDCARD, "SD Card", params.storage.gpsMode == GPS_MODE_SDCARD));
        server.sendContent(HTML::option(GPS_MODE_BT, "Bluetooth", params.storage.gpsMode == GPS_MODE_BT));
        server.sendContent(HTML_SELECT_CLOSE);

        server.sendContent(R"raw(</div></div><div class="col-sm-6"><div class="form-group">)raw");

        server.sendContent(HTML::select("SD Card log format", PARAM_GPS_LOG_FORMAT));
        server.sendContent(HTML::option(LOG_FORMAT_VBO, "VBO", params.storage.logFormat == LOG_FORMAT_VBO));
        server.sendContent(HTML::option(LOG_FORMAT_CSV_TRACKADDICT, "CSV (TrackAddict)", params.storage.logFormat == LOG_FORMAT_CSV_TRACKADDICT));
        server.sendContent(HTML_SELECT_CLOSE);
        server.sendContent(R"raw(</div></div></div></fieldset><fieldset><legend>GPS Receiver</legend><div class="row"><div class="col-sm-6"><div class="form-group">)raw");

        server.sendContent(HTML::select("GPS refresh rate", PARAM_GPS_RATE_HZ));
        server.sendContent(HTML::option(GPS_RATE_1_HZ, "1 Hz", params.storage.gpsRateHz == GPS_RATE_1_HZ));
        server.sendContent(HTML::option(GPS_RATE_5_HZ, "5 Hz", params.storage.gpsRateHz == GPS_RATE_5_HZ));
        server.sendContent(HTML::option(GPS_RATE_10_HZ, "10 Hz", params.storage.gpsRateHz == GPS_RATE_10_HZ));
        server.sendContent(HTML_SELECT_CLOSE);
        server.sendContent(R"raw(</div></div><div class="col-sm-6"><div class="form-group">)raw");

        server.sendContent(HTML::select("GPS baud rate", PARAM_GPS_BAUD_RATE));
        server.sendContent(HTML::option(GPS_RATE_38400, "38400", params.storage.gpsBaudRate == GPS_RATE_38400));
        server.sendContent(HTML::option(GPS_RATE_57600, "57600", params.storage.gpsBaudRate == GPS_RATE_57600));
        server.sendContent(HTML::option(GPS_RATE_115200, "115200", params.storage.gpsBaudRate == GPS_RATE_115200));
        server.sendContent(HTML_SELECT_CLOSE);
        server.sendContent(R"raw(</div></div></div></fieldset><fieldset><legend>NMEA Options</legend><div class="form-group">)raw");
        server.sendContent(HTML::checkbox("Enable NMEA TCP Server", PARAM_GPS_NMEA_TCP_ENABLED, params.storage.nmeaTcpEnabled));
        server.sendContent(R"raw(</div></fieldset><fieldset><legend>Presets</legend><div class="form-group">)raw");

        server.sendContent(HTML::select("Optimize GPS settings for", PARAM_GPS_OPTIMIZE_FOR));
        server.sendContent(HTML::option(GPS_OPTIMIZE_FOR_DEFAULT, "Default"));
        server.sendContent(HTML::option(GPS_OPTIMIZE_FOR_TRACKADDICT, "TrackAddict"));
        server.sendContent(HTML::option(GPS_OPTIMIZE_FOR_RACECHRONO, "RaceChrono"));
        server.sendContent(HTML::option(GPS_OPTIMIZE_FOR_RACETIME, "RaceTime"));
        server.sendContent(HTML_SELECT_CLOSE);

        server.sendContent("</div></fieldset>");

        server.sendContent(HTML_FORM_CLOSE);
        
        HTTP::endPage(server);

        return true;
    }

    bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
    {
        params.storage.gpsMode = server.arg(PARAM_GPS_MODE).toInt();
        params.storage.logFormat = server.arg(PARAM_GPS_LOG_FORMAT).toInt();
        params.storage.gpsRateHz = server.arg(PARAM_GPS_RATE_HZ).toInt();
        params.storage.gpsBaudRate = server.arg(PARAM_GPS_BAUD_RATE).toInt();
        params.storage.nmeaTcpEnabled = server.hasArg(PARAM_GPS_NMEA_TCP_ENABLED);
        params.storage.gpsOptimizeFor = server.arg(PARAM_GPS_OPTIMIZE_FOR).toInt();

        params.save();

        toast.append(HTML_SETTINGS_SUCCESS);

        HTTP::redirect(server, GPS_PAGE_BASE_URL);

        return true;
    }
};
