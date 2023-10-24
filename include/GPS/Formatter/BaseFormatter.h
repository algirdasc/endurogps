#pragma once

#include <SD.h>
#include <NMEAGPS.h>

const float rescaleGPS = 10000000.0;

class BaseFormatter
{
    protected:
        ulong startedAt;
        
        virtual void writeHeader(File file, gps_fix gpsFix) = 0;
        virtual void filepath(char *filePath, gps_fix gpsFix) = 0;

        String ts(gps_fix gpsFix, float fraction)
        {
            String timestamp = String((time_t) gpsFix.dateTime);
            timestamp += "." + String((int) ((fraction - (long) fraction) * 1000));

            return timestamp;
        }

        float accuracy(gps_fix gpsFix) { return gpsFix.lat_err() > gpsFix.lon_err() ? gpsFix.lat_err() : gpsFix.lon_err(); }
        float duration() { return (millis() - startedAt) / 1000.0; }
        
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

        virtual bool write(File file, gps_fix gpsFix) = 0;        
        virtual void close(File file) { file.close(); }
        
        virtual bool waitForValidLocation() 
        {
            return true;
        }        
};
