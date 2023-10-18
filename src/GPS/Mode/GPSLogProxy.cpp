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
        default:
            log_e("Unknown formatter provided");
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

void GPSLogProxy::handleLoop()
{
    if (!isStarted) {
        return;
    }

    while (nmeaGps.available(GPSSerial)) {        
        trace_all(SerialMonitor, nmeaGps, nmeaGps.read());

        // if (!logFormatter->write(logFile, gpsFix)) {
        //     stop();
        //     log_e("Failed to write GPS data");
        // }
    }
}
