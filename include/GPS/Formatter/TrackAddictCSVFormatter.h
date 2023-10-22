#pragma once

#include "BaseFormatter.h"

class TrackAddictCSVFormatter : public BaseFormatter
{
    public:
        File create();
        bool write(File file, gps_fix gpsFix);
        void writeHeader(File file);
};
