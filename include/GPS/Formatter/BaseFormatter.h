#pragma once

#include <SD.h>
#include <GPSfix.h>

class BaseFormatter
{
    public:
        virtual File create(gps_fix gpsFix) = 0;
        virtual bool write(File file, gps_fix gpsFix) = 0;
        virtual bool close(File file) = 0;
};
