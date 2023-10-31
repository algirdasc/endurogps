#include <NeoTime.h>
#include <FixedString.h>

#include "EnduroGPS.h"
#include "GPS/Formatter/TrackAddictCSVFormatter.h"

void TrackAddictCSVFormatter::filepath(char *filePath, gps_fix &gpsFix)
{
    sprintf(filePath, "/Log-20%i%02d%02d-%02d%02d%02d %s.csv",
            gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date,
            gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds,
            gpsSessionName);
}

size_t TrackAddictCSVFormatter::write(File &file, gps_fix &gpsFix)
{
    float duration = TrackAddictCSVFormatter::duration();

    FixedString16 utcTime;
    utcTime.appendFormat("%d.%03d", (time_t)gpsFix.dateTime, gpsFix.dateTime_ms());

    char buffer[1024];
    snprintf(buffer, 1024,
             "%.3f,%s,%d,%d,%.9f,%.9f,%.1f,%d,%.2f,%.1f,%.1f",
             duration,                          // Time                     %.3f
             utcTime.c_str(),                   // UTC Time                 %s
             lapNo,                             // Lap                      %d
             1,                                 // GPS Update               %d
             gpsFix.latitudeL() / RESCALE_GPS,  // Latitude                 %.9f
             gpsFix.longitudeL() / RESCALE_GPS, // Longitude                %.9f
             gpsFix.altitude(),                 // Altitude (m)             %.1f
             (uint)gpsFix.altitude_ft(),        // Altitude (ft)            %d
             gpsFix.speed_kph(),                // Speed km/h               %.1f
             gpsFix.heading(),                  // Heading                  %.1f
             gpsFix.lat_err(),                  // Latitude error (m)       %.1f
             gpsFix.lon_err(),                  // Longitude error (m)      %.1f
             gpsFix.alt_err()                   // Altitude error (m)       %.1f
    );

    return file.println(buffer);
}

void TrackAddictCSVFormatter::writeHeader(File &file, gps_fix &gpsFix)
{
    // User settings:
    // U1 - Units (U1 - kph, U0 - mph)

    file.println("# RaceRender Data: " DEVICE_NAME " (Mode -1)");
    file.println("# GPS: EXT: " DEVICE_NAME "; MODE: SD");
    // file.println("# User Settings: SL1;U1;AS1;LT0/1;EC0;VC-1;VQ3;VS0;VSOIS0;VIF1");
    file.println(R"raw("Time","UTC Time","Lap","GPS_Update","Latitude","Longitude","Altitude (m)","Altitude (ft)","Speed (Km/h)","Heading","Latitude error (m)","Longitude error (m)","Altitude error (m)")raw");
}

void TrackAddictCSVFormatter::close(File &file)
{
    file.println("# Session End");
    file.close();
}