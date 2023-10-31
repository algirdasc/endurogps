#pragma once

#include "BaseFormatter.h"

class TrackAddictCSVFormatter : public BaseFormatter
{
protected:
    void writeHeader(File &file, gps_fix &gpsFix);
    void filepath(char *filePath, gps_fix &gpsFix);

public:
    File create(gps_fix &gpsFix);
    size_t write(File &file, gps_fix &gpsFix);
    void close(File &file);
};
