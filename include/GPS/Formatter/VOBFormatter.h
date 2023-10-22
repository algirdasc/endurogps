#pragma once

#include "BaseFormatter.h"

class VOBFormatter : public BaseFormatter
{
    public:
        File create();
        bool write(File file, gps_fix gpsFix);
};
