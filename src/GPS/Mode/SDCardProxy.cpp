// #include "Global.h"
#include "GPS/Mode/SDCardProxy.h"

SDCardProxy::SDCardProxy()
{
    NMEAGPS nmeaGps;
    gps_fix gpsFix;
}

void SDCardProxy::formatter(uint formatter, const char *gpsSessionName)
{
    switch (formatter)
    {
    case LOG_FORMAT_VBO:
        logFormatter = new VOBFormatter();
        break;
    case LOG_FORMAT_CSV_TRACKADDICT:
        logFormatter = new TrackAddictCSVFormatter();
        break;
    }

    logFormatter->gpsSessionName = gpsSessionName;
}

void SDCardProxy::start()
{
    isProxyStarted = SD.begin();
}

void SDCardProxy::stop()
{
    if (logFile)
    {
        logFormatter->close(logFile);
    }

    isProxyStarted = false;
    isProxyFileCreated = false;
}

void SDCardProxy::handle(uint8_t *data, size_t size)
{
    if (!isProxyStarted)
    {
        return;
    }

    for (uint i = 0; i < size; i++)
    {
        nmeaGps.handle(data[i]);
    }

    if (nmeaGps.available())
    {
        gpsFix = nmeaGps.read();
        handleGpsFix(gpsFix);
    }

    if (nmeaGps.overrun())
    {
        nmeaGps.overrun(false);
        log_e("Too much data");
    }
}

void SDCardProxy::handleGpsFix(gps_fix gpsFix)
{
    if (!gpsFix.valid.date || !gpsFix.valid.time)
    {
        log_e("Waiting for valid GPS date/time to create file");

        return;
    }

    if (!isProxyFileCreated)
    {
        logFile = logFormatter->create(gpsFix);
        isProxyFileCreated = true;
    }

    if (!gpsFix.valid.location)
    {
        log_e("Invalid location, waiting...");

        return;
    }

    if (isProxyFileCreated && !logFormatter->write(logFile, gpsFix))
    {
        stop();
        // g_isRecording = false;
    }

    logFile.flush();
}
