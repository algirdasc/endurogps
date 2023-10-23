#include "GPS/Mode/GPSLogProxy.h"

GPSLogProxy::GPSLogProxy()
{
    NMEAGPS nmeaGps;
    gps_fix gpsFix;
}

void GPSLogProxy::formatter(uint formatter)
{
    switch (formatter) {
        case LOG_FORMAT_TRACK_ADDICT_CSV:
            logFormatter = new TrackAddictCSVFormatter();
            break;
        case LOG_FORMAT_VOB:
            logFormatter = new VOBFormatter();
            break;
    }
}

void GPSLogProxy::start()
{
    isStarted = true;
    fileCreated = false;    
}

void GPSLogProxy::stop()
{
    isStarted = false;
    logFormatter->close(logFile);
}

void GPSLogProxy::handle(char *data, size_t size)
{
    if (!isStarted) {
        return;
    }

    nmeaGps.available();

    uint8_t gpsData[GPS_MAX_BUFFER_SIZE];
    for (uint i = 0; i < size; i++) {
        nmeaGps.handle((uint8_t) data[i]);
    }

    if (nmeaGps.available()) {
        gpsFix = nmeaGps.read();
        if (!fileCreated && (!gpsFix.valid.date || !gpsFix.valid.time)) {
            log_d("Waiting for valid GPS date/time to create file");

            return;
        }

        if (!fileCreated) {
            logFile = logFormatter->create(gpsFix);
            fileCreated = true;
        }

        if (!logFormatter->write(logFile, gpsFix)) {
            stop();
            log_e("Failed to write GPS data");
        }
    }   

    if (nmeaGps.overrun()) {
        nmeaGps.overrun(false);
        log_e("Too much data");
    }
}
