#pragma once

#include "BaseFormatter.h"

class TrackAddictCSVFormatter : public BaseFormatter
{
    private:
        ulong startedAt;
        void writeHeader(File file);
    public:
        File create(gps_fix gpsFix);
        bool write(File file, gps_fix gpsFix);
        bool close(File file);        
};
