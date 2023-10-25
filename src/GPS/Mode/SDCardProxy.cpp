#include "GPS/Mode/SDCardProxy.h"

SDCardProxy::SDCardProxy()
{
    NMEAGPS nmeaGps;
    gps_fix gpsFix;
}

void SDCardProxy::formatter(uint formatter)
{
    switch (formatter) {
        case LOG_FORMAT_VBO:
            logFormatter = new VOBFormatter();
            break;
        case LOG_FORMAT_CSV_TRACKADDICT:
            logFormatter = new TrackAddictCSVFormatter();
            break;        
    }
}

void SDCardProxy::start()
{    
    isStarted = true;
}

void SDCardProxy::stop()
{
    if (logFile) {
        logFormatter->close(logFile);
    }

    isStarted = false;
    isFileCreated = false;
}

void SDCardProxy::handle(uint8_t *data, size_t size)
{
    if (!isStarted) {
        return;
    }

    for (uint i = 0; i < size; i++) {
        nmeaGps.handle(data[i]);
    }

    if (nmeaGps.available()) {
        gpsFix = nmeaGps.read();
        handleGpsFix(gpsFix);
    }   

    if (nmeaGps.overrun()) {
        nmeaGps.overrun(false);
        log_e("Too much data");
    }
}

void SDCardProxy::handleGpsFix(gps_fix gpsFix)
{
    if (!gpsFix.valid.date || !gpsFix.valid.time) {
        log_e("Waiting for valid GPS date/time to create file");

        return;
    }

    if (!isFileCreated) {
        logFile = logFormatter->create(gpsFix);
        isFileCreated = true;
    }

    if (!gpsFix.valid.location) {
        log_e("Invalid location, waiting...");

        return;
    }

    if (isFileCreated && !logFormatter->write(logFile, gpsFix)) {
        log_e("Failed to write GPS data");
        stop();        
    }
}
