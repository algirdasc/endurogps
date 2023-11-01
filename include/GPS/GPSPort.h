#pragma once

#include <Arduino.h>

#include "EnduroGPS.h"
#include "GPS/UBLOXMessages.h"

#define GPS_MODE_SDCARD 0
#define GPS_MODE_BT     1

#define GPS_RATE_1_HZ   1
#define GPS_RATE_5_HZ   5
#define GPS_RATE_10_HZ  10

#define GPS_OPTIMIZE_FOR_DEFAULT 0
#define GPS_OPTIMIZE_FOR_U_CENTER 1
#define GPS_OPTIMIZE_FOR_TRACKADDICT 2
#define GPS_OPTIMIZE_FOR_RACECHRONO_GGA 3
#define GPS_OPTIMIZE_FOR_RACECHRONO_VTG_ZDA 4
#define GPS_OPTIMIZE_FOR_RACETIME 5

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
