#include "GPS/Formatter/VOBFormatter.h"

File VOBFormatter::create()
{
    File file = SD.open("/test.vob", FILE_APPEND);
    return file;
}

bool VOBFormatter::write(File file, gps_fix gpsFix)
{
    return 1;
}
