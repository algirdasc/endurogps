#pragma once

#include "BaseFormatter.h"
#include "GPSTime.h"

class TrackAddictCSVFormatter : public BaseFormatter
{
    private:
        ulong createdAt;
    public:
        File create(gps_fix gpsFix);
        bool write(File file, gps_fix gpsFix);
        bool close(File file);
        void writeHeader(File file);
};
