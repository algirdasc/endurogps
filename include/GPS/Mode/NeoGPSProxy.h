#pragma once

#include <SD.h>
#include "Params.h"
#include "Globals.h"

#include "GPS/NeoGPS.h"
#include "GPS/Coordinates.h"
#include "GPS/Mode/BaseProxy.h"
#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include "GPS/Formatter/VBOFormatter.h"

class NeoGPSProxy : public BaseProxy
{
    private:
        ubloxNMEA gps;        
        BaseFormatter *logFormatter;
        File logFile;        
        Line startFinishLine;
        bool isProxyFileCreated = false;

        void handleFormatter(gps_fix gpsFix);
        Line fromGpsFix(gps_fix g1, gps_fix g2);        
    public:
        NeoGPSProxy();

        gps_fix gpsFix;
        gps_fix lastGpsFix;

        void formatter(uint formatter, const char *gpsSessionName);
        void startFinishLineCoordinates(int32_t lat1, int32_t lon1, int32_t lat2, int32_t lon2);
        void start();
        void stop();
        void handleProxy(uint8_t *data, size_t size);
};
