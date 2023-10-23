#pragma once

#include <WebServer.h>

#include "Params.h"
#include "GPS/GPSPort.h"
#include "HTTP/HTTPCodes.h"


const char GPS_PAGE_BASE_URL[] PROGMEM = "/settings/gps";
const char *GPS_PAGE_MODE_LABELS[] PROGMEM = {"Local (SDCard)", "Bluetooth"}; 
const char *GPS_PAGE_LOG_FORMAT_LABELS[] PROGMEM = {"TrackAddict CSV", "VBO"};
const char *GPS_PAGE_BAUD_RATE_VALUES[] PROGMEM = {"38400", "57600", "115200"};
const char *GPS_PAGE_OPTIMIZE_FOR_LABELS[] PROGMEM = {"", "RaceTime", "RaceChrono", "TractAddict"};
const char *GPS_PAGE_RATE_HZ_VALUES[] PROGMEM = {"1", "5", "10"};


class SettingsGPSHandler : public RequestHandler
{   
    private:
        Params params;
        
        String logFormatValues[2] = {String(LOG_FORMAT_TRACK_ADDICT_CSV), String(LOG_FORMAT_VOB)};

        String gpsBaudRateValues[3] = {"38400", "57600", "115200"};

        String gpsRateHzValues[3] = {"1", "5", "10"};

        String gpsPowerSaveValues[2] = {"1800", "3600"};
        String gpsPowerSaveLabels[2] = {"30 minutes", "1 hour"};

        String nmeaTalkertIDValues[4] = {String(GPSPORT_MAINTALKER_GP), String(GPSPORT_MAINTALKER_GP_GPSONLY)};
        String nmeaTalkertIDLabels[4] = {"GP", "GP (GPS only)"};

        String nmeaSVChannelsValues[2] = {"0", "8"};
        String nmeaSVChannelsLabels[2] = {"All", "8 channels"};

        String nmeaOptimizeForValues[4] = {"", "racetime", "racechrono", "trackaddict"};

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

            htmlOutput += R"raw(<fieldset><legend>Logging</legend><div class="pure-g">)raw";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select2(F("Mode"), GPS_MODE, {}, GPS_PAGE_MODE_LABELS, 2, String(params.storage.gpsMode));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select2(F("Format"), GPS_LOG_FORMAT, logFormatValues, GPS_PAGE_LOG_FORMAT_LABELS, 2, String(params.storage.logFormat));
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>GPS Receiver</legend><div class="pure-g">)raw";
            
            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select2(F("GPS Refresh rate"), GPS_RATE_HZ, gpsRateHzValues, GPS_PAGE_RATE_HZ_VALUES, 3, String(params.storage.gpsRateHz));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select2(F("GPS Baud rate"), T_GPS_BAUD_RATE, gpsBaudRateValues, GPS_PAGE_BAUD_RATE_VALUES, 3, String(params.storage.gpsBaudRate));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select(F("Powersaving"), GPS_POWER_SAVE, gpsPowerSaveValues, gpsPowerSaveLabels, 2, String(params.storage.gpsPowerSave));
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>NMEA Options</legend><div class="pure-g">)raw";
            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox(F("GxGSV"), GPS_NMEA_GSV, params.storage.nmeaGSV);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox(F("GxGSA"), GPS_NMEA_GSA, params.storage.nmeaGSA);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox(F("GxGBS"), GPS_NMEA_GBS, params.storage.nmeaGBS);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox(F("GxGLL"), GPS_NMEA_GLL, params.storage.nmeaGLL);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox(F("GxVTG"), GPS_NMEA_VTG, params.storage.nmeaVTG);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select(F("Main Talker ID"), GPS_NMEA_MAIN_TALKER, nmeaTalkertIDValues, nmeaTalkertIDLabels, 2, String(params.storage.nmeaMainTalker));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select(F("SVs per Talker ID"), GPS_NMEA_SV_CHANNELS, nmeaSVChannelsValues, nmeaSVChannelsLabels, 2, String(params.storage.nmeaSVChannels));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-1">)raw";
            htmlOutput += HTML::checkbox(F("Enable NMEA TCP Server"), GPS_NMEA_TCP_ENABLED, params.storage.nmeaTcpEnabled);
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>Presets</legend><div class="pure-g">)raw";

            htmlOutput += R"raw(<div class="pure-u-1-1">)raw";
            htmlOutput += HTML::select2(F("Optimize settings for"), F("optimizeFor"), nmeaOptimizeForValues, GPS_PAGE_OPTIMIZE_FOR_LABELS, 4);
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += HTML::formEnd();

            server.send(HTTP_CODE_OK, contentTypeHtml, Template::generateBody(htmlOutput, F("GPS Settings")));

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            params.storage.gpsMode = server.arg(GPS_MODE).toInt();
            params.storage.logFormat = server.arg(GPS_LOG_FORMAT).toInt();
            params.storage.gpsRateHz = server.arg(GPS_RATE_HZ).toInt();
            params.storage.gpsBaudRate = server.arg(T_GPS_BAUD_RATE).toInt();
            params.storage.gpsPowerSave = server.arg(GPS_POWER_SAVE).toInt();
            params.storage.nmeaGSV = server.hasArg(GPS_NMEA_GSV);
            params.storage.nmeaGSA = server.hasArg(GPS_NMEA_GSA);
            params.storage.nmeaGBS = server.hasArg(GPS_NMEA_GBS);
            params.storage.nmeaGLL = server.hasArg(GPS_NMEA_GLL);
            params.storage.nmeaVTG = server.hasArg(GPS_NMEA_VTG);
            params.storage.nmeaMainTalker = server.arg(GPS_NMEA_MAIN_TALKER).toInt();
            params.storage.nmeaSVChannels = server.arg(GPS_NMEA_SV_CHANNELS).toInt();
            params.storage.nmeaTcpEnabled = server.hasArg(GPS_NMEA_TCP_ENABLED);
            
            params.save();

            Template::redirect(server, GPS_PAGE_BASE_URL);

            return true;
        }
};
