#pragma once

#include "BaseFormatter.h"

class VOBFormatter : public BaseFormatter
{
    public:
        File create(gps_fix gpsFix);
        bool write(File file, gps_fix gpsFix);
        bool close(File file);
};
