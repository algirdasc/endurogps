#pragma once

#include <WebServer.h>
#include "GPS/GPSPort.h"
#include "Params.h"

class SettingsGPSHandler : public RequestHandler
{   
    private:
        Params params;
        
        String gpsModeValues[2] = {String(GPS_MODE_CSV), String(GPS_MODE_BLE)};
        String gpsModeLabels[2] = {"Local (SDCard)", "Bluetooth"};

        String logFormatValues[2] = {String(LOG_FORMAT_TRACK_ADDICT_CSV), String(LOG_FORMAT_VOB)};
        String logFormatLabels[2] = {"TrackAddict CSV", "VBO"};

        String gpsBaudRateValues[3] = {"38400", "57600", "115200"};
        String gpsBaudRateLabels[3] = {"38k4", "57k6", "115k2"};

        String gpsRateHzValues[3] = {"1", "5", "10"};
        String gpsRateHzLabels[3] = {"1 Hz", "5 Hz", "10 Hz"};

        String gpsPowerSaveValues[2] = {"1800", "3600"};
        String gpsPowerSaveLabels[2] = {"30 minutes", "1 hour"};

        String nmeaTalkertIDValues[4] = {String(GPSPORT_MAINTALKER_GP), String(GPSPORT_MAINTALKER_GP_GPSONLY)};
        String nmeaTalkertIDLabels[4] = {"GP", "GP (GPS only)"};

        String nmeaSVChannelsValues[2] = {"0", "8"};
        String nmeaSVChannelsLabels[2] = {"All", "8 channels"};

        String nmeaOptimizeForValues[4] = {"", "racetime", "racechrono", "trackaddict"};
        String nmeaOptimizeForLabels[4] = {"", "RaceTime", "RaceChrono", "TractAddict"};

    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/settings/gps";
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
            htmlOutput += HTML::select("Mode", "gpsMode", gpsModeValues, gpsModeLabels, 2, String(params.storage.gpsMode));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select("Format", "logFormat", logFormatValues, logFormatLabels, 2, String(params.storage.logFormat));
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>GPS Receiver</legend><div class="pure-g">)raw";
            
            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select("GPS Refresh rate", "gpsRateHz", gpsRateHzValues, gpsRateHzLabels, 3, String(params.storage.gpsRateHz));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select("GPS Baud rate", "gpsBaudRate", gpsBaudRateValues, gpsBaudRateLabels, 3, String(params.storage.gpsBaudRate));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-3">)raw";
            htmlOutput += HTML::select("Powersaving", "gpsPowerSave", gpsPowerSaveValues, gpsPowerSaveLabels, 2, String(params.storage.gpsPowerSave));
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>NMEA Options</legend><div class="pure-g">)raw";
            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox("GxGSV", "nmeaGSV", params.storage.nmeaGSV);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox("GxGSA", "nmeaGSA", params.storage.nmeaGSA);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox("GxGBS", "nmeaGBS", params.storage.nmeaGBS);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox("GxGLL", "nmeaGLL", params.storage.nmeaGLL);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-5">)raw";
            htmlOutput += HTML::checkbox("GxVTG", "nmeaVTG", params.storage.nmeaVTG);
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select("Main Talker ID", "nmeaMainTalker", nmeaTalkertIDValues, nmeaTalkertIDLabels, 2, String(params.storage.nmeaMainTalker));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-2">)raw";
            htmlOutput += HTML::select("SVs per Talker ID", "nmeaSVChannels", nmeaSVChannelsValues, nmeaSVChannelsLabels, 2, String(params.storage.nmeaSVChannels));
            htmlOutput += "</div>";

            htmlOutput += R"raw(<div class="pure-u-1-1">)raw";
            htmlOutput += HTML::checkbox("Enable NMEA TCP Server", "nmeaTcpEnabled", params.storage.nmeaTcpEnabled);
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += R"raw(<fieldset><legend>Presets</legend><div class="pure-g">)raw";

            htmlOutput += R"raw(<div class="pure-u-1-1">)raw";
            htmlOutput += HTML::select("Optimize settings for", "optimizeFor", nmeaOptimizeForValues, nmeaOptimizeForLabels, 4);
            htmlOutput += "</div>";

            htmlOutput += "</div></fieldset>";

            htmlOutput += HTML::formEnd();

            server.send(200, contentTypeHtml, Template::generateBody(htmlOutput, "GPS Settings"));

            return true;
        }

        bool handlePost(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            params.storage.gpsMode = server.arg("gpsMode").toInt();
            params.storage.logFormat = server.arg("logFormat").toInt();
            params.storage.gpsRateHz = server.arg("gpsRateHz").toInt();
            params.storage.gpsBaudRate = server.arg("gpsBaudRate").toInt();
            params.storage.gpsPowerSave = server.arg("gpsPowerSave").toInt();
            params.storage.nmeaGSV = server.hasArg("nmeaGSV");
            params.storage.nmeaGSA = server.hasArg("nmeaGSA");
            params.storage.nmeaGBS = server.hasArg("nmeaGBS");
            params.storage.nmeaGLL = server.hasArg("nmeaGLL");
            params.storage.nmeaVTG = server.hasArg("nmeaVTG");
            params.storage.nmeaMainTalker = server.arg("nmeaMainTalker").toInt();
            params.storage.nmeaSVChannels = server.arg("nmeaSVChannels").toInt();
            params.storage.nmeaTcpEnabled = server.hasArg("nmeaTcpEnabled");
            
            params.save();

            Template::redirect(server, "/settings/gps");

            return true;
        }
};
