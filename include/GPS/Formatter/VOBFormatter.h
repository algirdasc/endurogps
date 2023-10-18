#ifndef VOB_FORMATTER_H
#define VOB_FORMATTER_H

#include "BaseFormatter.h"

class VOBFormatter : public BaseFormatter
{
    public:
        File create();
        bool write(File file, gps_fix gpsFix);
};

#endif