#ifndef BASE_FORMAT_H
#define BASE_FORMAT_H

#include <SD.h>
#include <GPSfix.h>

class BaseFormatter
{
    public:
        virtual File create() = 0;
        virtual bool write(File file, gps_fix gpsFix) = 0;
};

#endif
