#pragma once

#include <GPSfix_cfg.h>
#include <NMEAGPS.h>
#include <SD.h>

#include "EnduroGPS.h"
#include "Params.h"

#include "GPS/Mode/BaseProxy.h"
#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include "GPS/Formatter/VBOFormatter.h"

class GPSLogProxy : public BaseProxy
{
    private:
        NMEAGPS nmeaGps;
        gps_fix gpsFix;
        BaseFormatter *logFormatter;
        File logFile;        
        bool isFileCreated = false;
        void handleGpsFix(gps_fix gpsFix);
    public:
        GPSLogProxy();
        
        void formatter(uint formatter);
        void start();
        void stop();
        void handle(uint8_t *data, size_t size);
};
