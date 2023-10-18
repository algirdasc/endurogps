#ifndef TRACK_ADDICT_CSV_FORMATTER_H
#define TRACK_ADDICT_CSV_FORMATTER_H

#include "BaseFormatter.h"

class TrackAddictCSVFormatter : public BaseFormatter
{
    public:
        File create();
        bool write(File file, gps_fix gpsFix);
        void writeHeader(File file);
};

#endif
