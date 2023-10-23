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
//#define NMEAGPS_DERIVED_TYPES
//#define NMEAGPS_PARSE_PROPRIETARY
//#define NMEAGPS_PARSE_MFR_ID

// #define GPS_FIX_LAT_ERR
// #define GPS_FIX_LON_ERR
// #undef NEOGPS_PACKED_DATA
// #define USE_FLOAT

#define TIME_EPOCH_MODIFIABLE
//#define NMEAGPS_TIMESTAMP_FROM_PPS
#define NMEAGPS_TIMESTAMP_FROM_INTERVAL
#define NMEAGPS_RECOGNIZE_ALL

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
