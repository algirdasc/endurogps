#pragma once

#include <Arduino.h>

#include "EnduroGPS.h"
#include "GPS/UBLOXMessages.h"

#define GPS_MODE_SDCARD 0
#define GPS_MODE_BT     1
#define GPS_RATE_1_HZ   1
#define GPS_RATE_5_HZ   5
#define GPS_RATE_10_HZ  10
#define GPS_RATE_38400  38400
#define GPS_RATE_57600  57600
#define GPS_RATE_115200 115200
#define GPS_OPTIMIZE_FOR_DEFAULT 0
#define GPS_OPTIMIZE_FOR_TRACKADDICT 1
#define GPS_OPTIMIZE_FOR_RACECHRONO 2
#define GPS_OPTIMIZE_FOR_RACETIME 3

class GPSPort
{    
    public:
        void initialize();
        void pushMessage(const char message[], uint size);
        void stop();
        void start();
        void setBaudrate(uint32_t baudRate);
        void setRefreshRate(uint refreshRate);
        void setPowerSave(uint timeS);
        void optimizeFor(uint app);
};
