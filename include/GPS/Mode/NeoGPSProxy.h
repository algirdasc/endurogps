#pragma once

#include <SD.h>
#include "Params.h"
#include "Globals.h"

#include "GPS/NeoGPS.h"
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
        bool isProxyFileCreated = false;
        void handleFormatter(gps_fix gpsFix);
    public:
        NeoGPSProxy();

        gps_fix gpsFix;

        void formatter(uint formatter, const char *gpsSessionName);
        void start();
        void stop();
        void handleProxy(uint8_t *data, size_t size);
};
