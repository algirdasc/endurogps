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

    if (Coordinates::intersects(startFinishLine, fromGpsFix(lastGpsFix, gpsFix)))
    {
        logFormatter->newLap();
    }

    if (isProxyFileCreated && !logFormatter->write(logFile, gpsFix))
    {
        stop();
    }

    logFile.flush();
    lastGpsFix = gpsFix;
}

Line NeoGPSProxy::fromGpsFix(gps_fix g1, gps_fix g2)
{
    Line line = {.p1 = {.lat = g1.latitudeL(), .lon = g1.longitudeL()}, .p2 = {.lat = g2.latitudeL(), .lon = g2.longitudeL()}};

    return line;
}

void NeoGPSProxy::startFinishLineCoordinates(int32_t lat1, int32_t lon1, int32_t lat2, int32_t lon2)
{
    startFinishLine = {.p1 = {.lat = lat1, .lon = lon1}, .p2 = {.lat = lat2, .lon = lon2}};
}
