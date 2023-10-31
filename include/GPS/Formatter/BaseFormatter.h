#pragma once

#include <SD.h>
#include <NMEAGPS.h>

#define RESCALE_GPS 10000000.0
#define LOG_FORMAT_VBO 0
#define LOG_FORMAT_CSV_TRACKADDICT 1

class BaseFormatter
{
protected:
    ulong startedAt;

    virtual void writeHeader(File file, gps_fix gpsFix) = 0;
    virtual void filepath(char *filePath, gps_fix gpsFix) = 0;

    String ts(gps_fix gpsFix, float fraction)
    {
        String timestamp = String((time_t)gpsFix.dateTime);
        timestamp += "." + String((int)((fraction - (long)fraction) * 1000));

        return timestamp;
    }

    float latLonAccuracy(gps_fix gpsFix) { return gpsFix.lat_err() > gpsFix.lon_err() ? gpsFix.lat_err() : gpsFix.lon_err(); }
    float duration() { return (millis() - startedAt) / 1000.0; }
    int durationFraction(int multiplicator = 1000)
    {
        float fraction = duration();

        return (int)((fraction - (long)fraction) * multiplicator);
    }

public:
    File create(gps_fix gpsFix)
    {
        char filePath[128];
        filepath(filePath, gpsFix);

        File file = SD.open(filePath, FILE_WRITE, true);
        startedAt = millis();

        writeHeader(file, gpsFix);

        return file;
    }

    const char *gpsSessionName;

    virtual size_t write(File file, gps_fix gpsFix);
    virtual void close(File file) { file.close(); }
};
