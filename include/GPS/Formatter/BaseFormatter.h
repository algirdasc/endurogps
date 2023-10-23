#pragma once

#include <SD.h>
#include <NMEAGPS.h>

const float rescaleGPS = 10000000.0;

class BaseFormatter
{
    public:
        virtual File create(gps_fix gpsFix) = 0;
        virtual bool write(File file, gps_fix gpsFix) = 0;
        virtual bool close(File file) = 0;
        const char *ts(gps_fix gpsFix, float fraction)
        {
            String timestamp = String((time_t) gpsFix.dateTime);
            timestamp += "." + String((int) ((fraction - (long) fraction) * 1000));

            return timestamp.c_str();
        }
        const float accuracy(gps_fix gpsFix)
        {
            return gpsFix.lat_err() > gpsFix.lon_err() ? gpsFix.lat_err() : gpsFix.lon_err();
        }
};
