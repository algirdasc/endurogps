#pragma once

#include <SD.h>
#include <GPSfix.h>

class BaseFormatter
{
    public:
        virtual File create() = 0;
        virtual bool write(File file, gps_fix gpsFix) = 0;
};
