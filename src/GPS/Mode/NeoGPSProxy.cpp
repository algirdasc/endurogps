#include "GPS/Mode/NeoGPSProxy.h"

NeoGPSProxy::NeoGPSProxy()
{
    ubloxNMEA gps;
    gps_fix gpsFix;
}

void NeoGPSProxy::formatter(uint formatter, const char *gpsSessionName)
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

void NeoGPSProxy::start()
{
    isProxyStarted = SD.begin();
    g_isRecording = isProxyStarted;
}

void NeoGPSProxy::stop()
{
    if (logFile)
    {
        logFormatter->close(logFile);
    }

    isProxyStarted = false;
    isProxyFileCreated = false;
    g_isRecording = false;
}

void NeoGPSProxy::handleProxy(uint8_t *data, size_t size)
{
    if (!isProxyStarted)
    {
        return;
    }

    for (uint i = 0; i < size; i++)
    {
        gps.handle(data[i]);
    }

    if (gps.available())
    {
        gpsFix = gps.read();
        handleFormatter(gpsFix);
    }

    if (gps.overrun())
    {
        gps.overrun(false);
        log_e("Too much data");
    }
}

void NeoGPSProxy::handleFormatter(gps_fix gpsFix)
{      
    if (!isProxyFileCreated && (!gpsFix.valid.date || !gpsFix.valid.time))
    {
        log_e("Waiting for valid GPS date/time to create file");

        return;
    }
    
    if (!isProxyFileCreated)
    {
        logFile = logFormatter->create(gpsFix);
        isProxyFileCreated = true;
    }

    g_isLocationValid = gpsFix.valid.location;
    if (!g_isLocationValid)
    {
        return;
    }

    if (isProxyFileCreated && !logFormatter->write(logFile, gpsFix))
    {
        stop();
    }

    logFile.flush();
}
