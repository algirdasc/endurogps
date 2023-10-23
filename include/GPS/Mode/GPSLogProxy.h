#pragma once

#include <GPSfix_cfg.h>
#include <NMEAGPS.h>
#include <SD.h>
#include "EnduroGPS.h"
#include "Params.h"

#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include "GPS/Formatter/VBOFormatter.h"

class GPSLogProxy
{
    private:
        NMEAGPS nmeaGps;
        gps_fix gpsFix;
        BaseFormatter *logFormatter;
        File logFile;
        bool isStarted = false;
        bool fileCreated = false;

    public:
        GPSLogProxy();
        
        void formatter(uint formatter);
        void start();
        void stop();
        void handle(char *data, size_t size);
};
