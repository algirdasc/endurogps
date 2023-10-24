#pragma once

#include <WebServer.h>

#include "Params.h"
#include "GPS/GPSPort.h"
#include "HTTP/HTTPCodes.h"


const char GPS_PAGE_BASE_URL[] PROGMEM = "/settings/gps";
const char *GPS_PAGE_MODE_VALUES[] PROGMEM = {"0", "1"};
const char *GPS_PAGE_MODE_LABELS[] PROGMEM = {"Local (SDCard)", "Bluetooth"}; 
const char *GPS_PAGE_LOG_FORMAT_VALUES[] PROGMEM = {"0", "1"};
const char *GPS_PAGE_LOG_FORMAT_LABELS[] PROGMEM = {"TrackAddict CSV", "VBO"};
const char *GPS_PAGE_BAUD_RATE_VALUES[] PROGMEM = {"38400", "57600", "115200"};
const char *GPS_PAGE_OPTIMIZE_FOR_LABELS[] PROGMEM = {"", "RaceTime", "RaceChrono", "TrackAddict"};
const char *GPS_PAGE_RATE_HZ_VALUES[] PROGMEM = {"1", "5", "10"};
const char *GPS_PAGE_SV_CHANNELS_VALUES[] PROGMEM = {"0", "8", "12", "16"};
const char *GPS_PAGE_SV_CHANNELS_LABELS[] PROGMEM = {"Standard", "8 channels", "12 channels", "16 channels"};
const char *GPS_PAGE_TALKER_ID_VALUES[] PROGMEM = {"0", "1"};
const char *GPS_PAGE_TALKER_ID_LABELS[] PROGMEM = {"GP", "GP (GPS only)"};

const char GPS_PAGE_REFRESH_RATE[] PROGMEM = "GPS Refresh rate, Hz";
const char GPS_PAGE_BAUD_RATE[] PROGMEM = "GPS Baud rate";
const char GPS_PAGE_OPTIMIZE_SETTINGS[] PROGMEM = "Optimize settings for";
const char GPS_PAGE_MODE[] PROGMEM = "Mode";
const char GPS_PAGE_LOG_FORMAT[] PROGMEM = "Format";
const char GPS_PAGE_SV_PER_TALKER[] PROGMEM = "SVs per Talker ID";
const char GPS_PAGE_MAIN_TALKER_ID[] PROGMEM = "Main Talker ID";
const char GPS_PAGE_NMEA_SERVER[] PROGMEM = "Enable NMEA TCP Server";
const char GPS_PAGE_GxGSV[] PROGMEM = "GxGSV";
const char GPS_PAGE_GxGSA[] PROGMEM = "GxGSA";
const char GPS_PAGE_GxGBS[] PROGMEM = "GxGBS";
const char GPS_PAGE_GxGLL[] PROGMEM = "GxGLL";
const char GPS_PAGE_GxVTG[] PROGMEM = "GxVTG";


class SettingsGPSHandler : public RequestHandler
{   
    private:
        Params params;

    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == GPS_PAGE_BASE_URL;
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            params.load();

            if (requestMethod == HTTP_POST) {
                return handlePost(server, requestMethod, requestUri);
            }

            String htmlOutput = HTML::formStart();            

            htmlOutput += F(R"raw(<fieldset><legend>Logging</legend><div class="pure-g"><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_MODE, PARAM_GPS_MODE, GPS_PAGE_MODE_VALUES, GPS_PAGE_MODE_LABELS, 2, String(params.storage.gpsMode));
            htmlOutput += F(R"raw(</div><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_LOG_FORMAT, PARAM_GPS_LOG_FORMAT, GPS_PAGE_LOG_FORMAT_VALUES, GPS_PAGE_LOG_FORMAT_LABELS, 2, String(params.storage.logFormat));
            htmlOutput += F(R"raw(</div></div></fieldset><fieldset><legend>GPS Receiver</legend><div class="pure-g"><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_REFRESH_RATE, PARAM_GPS_RATE_HZ, GPS_PAGE_RATE_HZ_VALUES, GPS_PAGE_RATE_HZ_VALUES, 3, String(params.storage.gpsRateHz));
            htmlOutput += F(R"raw(</div><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_BAUD_RATE, PARAM_GPS_BAUD_RATE, GPS_PAGE_BAUD_RATE_VALUES, GPS_PAGE_BAUD_RATE_VALUES, 3, String(params.storage.gpsBaudRate));
            htmlOutput += F(R"raw(</div></fieldset><fieldset><legend>NMEA Options</legend><div class="pure-g"><div class="pure-u-1-5">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_GxGSV, PARAM_GPS_NMEA_GSV, params.storage.nmeaGSV);
            htmlOutput += F(R"raw(</div><div class="pure-u-1-5">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_GxGSA, PARAM_GPS_NMEA_GSA, params.storage.nmeaGSA);
            htmlOutput += F(R"raw(</div><div class="pure-u-1-5">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_GxGBS, PARAM_GPS_NMEA_GBS, params.storage.nmeaGBS);
            htmlOutput += F(R"raw(</div><div class="pure-u-1-5">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_GxGLL, PARAM_GPS_NMEA_GLL, params.storage.nmeaGLL);
            htmlOutput += F(R"raw(</div><div class="pure-u-1-5">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_GxVTG, PARAM_GPS_NMEA_VTG, params.storage.nmeaVTG);
            htmlOutput += F(R"raw(</div><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_MAIN_TALKER_ID, PARAM_GPS_NMEA_MAIN_TALKER, GPS_PAGE_TALKER_ID_VALUES, GPS_PAGE_TALKER_ID_LABELS, 2, String(params.storage.nmeaMainTalker));
            htmlOutput += F(R"raw(</div><div class="pure-u-1-2">)raw");
            htmlOutput += HTML::select(GPS_PAGE_SV_PER_TALKER, PARAM_GPS_NMEA_SV_CHANNELS, GPS_PAGE_SV_CHANNELS_VALUES, GPS_PAGE_SV_CHANNELS_LABELS, 4, String(params.storage.nmeaSVChannels));
            htmlOutput += F(R"raw(</div><div class="pure-u-1-1">)raw");
            htmlOutput += HTML::checkbox(GPS_PAGE_NMEA_SERVER, PARAM_GPS_NMEA_TCP_ENABLED, params.storage.nmeaTcpEnabled);
            htmlOutput += F(R"raw(</div></div></fieldset><fieldset><legend>Presets</legend><div class="pure-g"><div class="pure-u-1-1">)raw");
            htmlOutput += HTML::select(GPS_PAGE_OPTIMIZE_SETTINGS, "optimizeFor", GPS_PAGE_OPTIMIZE_FOR_LABELS, GPS_PAGE_OPTIMIZE_FOR_LABELS, 4);
            htmlOutput += F("</div></div></fieldset>");

            htmlOutput += HTML::formEnd();

            server.send(HTTP_CODE_OK, contentTypeHtml, Template::generateBody(htmlOutput, F("GPS Settings")));

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            params.storage.gpsMode = server.arg(PARAM_GPS_MODE).toInt();
            params.storage.logFormat = server.arg(PARAM_GPS_LOG_FORMAT).toInt();
            params.storage.gpsRateHz = server.arg(PARAM_GPS_RATE_HZ).toInt();
            params.storage.gpsBaudRate = server.arg(PARAM_GPS_BAUD_RATE).toInt();
            params.storage.nmeaGSV = server.hasArg(PARAM_GPS_NMEA_GSV);
            params.storage.nmeaGSA = server.hasArg(PARAM_GPS_NMEA_GSA);
            params.storage.nmeaGBS = server.hasArg(PARAM_GPS_NMEA_GBS);
            params.storage.nmeaGLL = server.hasArg(PARAM_GPS_NMEA_GLL);
            params.storage.nmeaVTG = server.hasArg(PARAM_GPS_NMEA_VTG);
            params.storage.nmeaMainTalker = server.arg(PARAM_GPS_NMEA_MAIN_TALKER).toInt();
            params.storage.nmeaSVChannels = server.arg(PARAM_GPS_NMEA_SV_CHANNELS).toInt();
            params.storage.nmeaTcpEnabled = server.hasArg(PARAM_GPS_NMEA_TCP_ENABLED);
            
            params.save();

            Template::redirect(server, GPS_PAGE_BASE_URL);

            return true;
        }
};
