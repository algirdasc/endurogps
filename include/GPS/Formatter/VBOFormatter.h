#pragma once

#include "BaseFormatter.h"

class VOBFormatter : public BaseFormatter
{
private:
    uint lastRateCalculationAt = 0;
    uint writeCount = 0;
    float updateRate = 0.0;

protected:
    void writeHeader(File file, gps_fix gpsFix);
    void filepath(char *filePath, gps_fix gpsFix);

public:
    File create(gps_fix gpsFix);
    size_t write(File file, gps_fix gpsFix);
};
