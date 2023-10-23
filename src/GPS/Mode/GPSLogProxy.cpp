#include "GPS/Mode/GPSLogProxy.h"
#include "Streamers.h"

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
    logFile = logFormatter->create();
}

void GPSLogProxy::stop()
{
    isStarted = false;
    logFile.close();
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
        trace_all(SerialMonitor, nmeaGps, gpsFix);
        if (!logFormatter->write(logFile, gpsFix)) {
            stop();
            log_e("Failed to write GPS data");
        }
    }   

    if (nmeaGps.overrun()) {
        nmeaGps.overrun(false);
        log_e("Too much data");
    } 

    // if (!logFormatter->write(logFile, gpsFix)) {
    //     stop();
    //     log_e("Failed to write GPS data");
    // }
}
