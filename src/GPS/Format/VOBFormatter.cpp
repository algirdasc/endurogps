#include "GPS/Formatter/VOBFormatter.h"

File VOBFormatter::create(gps_fix gpsFix)
{
    File file = SD.open("/test.vob", FILE_APPEND);
    
    return file;
}

bool VOBFormatter::write(File file, gps_fix gpsFix)
{
    return 1;
}

bool VOBFormatter::close(File file)
{
    file.close();

    return 1;
}