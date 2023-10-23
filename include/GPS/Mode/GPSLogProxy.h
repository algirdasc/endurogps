#pragma once

#include <GPSfix_cfg.h>
#include <NMEAGPS.h>
#include <Streamers.h>
#include <SD.h>
#include "EnduroGPS.h"
#include "Params.h"

#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include "GPS/Formatter/VOBFormatter.h"

//#define NMEAGPS_KEEP_NEWEST_FIXES true
//#define NMEAGPS_PARSING_SCRATCHPAD
//#define NMEAGPS_TIMESTAMP_FROM_INTERVAL
//#define NMEAGPS_DERIVED_TYPES
//#define NMEAGPS_PARSE_PROPRIETARY
//#define NMEAGPS_PARSE_MFR_ID
#define TIME_EPOCH_MODIFIABLE
#undef NEOGPS_PACKED_DATA

class GPSLogProxy
{
    private:
        NMEAGPS nmeaGps;
        gps_fix gpsFix;
        BaseFormatter *logFormatter;
        File logFile;
        bool isStarted = false;

    public:
        GPSLogProxy();
        
        void formatter(uint formatter);
        void start();
        void stop();
        void handle(char *data, size_t size);
};
