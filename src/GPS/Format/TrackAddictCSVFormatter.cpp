#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include <NeoTime.h>

void TrackAddictCSVFormatter::filepath(char *filePath, gps_fix gpsFix)
{
    sprintf(filePath, "/Log-20%i%02d%02d-%02d%02d%02d %s.csv",
            gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date,
            gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds,
            gpsSessionName);
}

size_t TrackAddictCSVFormatter::write(File file, gps_fix gpsFix)
{
    float duration = TrackAddictCSVFormatter::duration();

    Serial.println(ts(gpsFix, duration));

    char buffer[2048];
    snprintf(buffer, 2048,
             "%.3f,%s,%i,%.3f,%.9f,%.9f,%.1f,%i,%.1f,%.1f,%.1f",
             duration,                          // Time                     %.3f
             ts(gpsFix, duration),              // UTC Time                 %s
             1,                                 // GPS Update               %i
             (float)gpsFix.dateTime_cs / 1000,  // GPS Delay                %.3f
             gpsFix.latitudeL() / RESCALE_GPS,  // Latitude                 %.9f
             gpsFix.longitudeL() / RESCALE_GPS, // Longitude                %.9f
             gpsFix.altitude(),                 // Altitude (m)             %.1f
             (uint)gpsFix.altitude_ft(),        // Altitude (ft)            %i
             gpsFix.speed_kph(),                // Speed km/h               %.1f
             gpsFix.heading(),                  // Heading                  %.1f
             latLonAccuracy(gpsFix)             // Accuracy (m)             %.1f
    );

    Serial.println(buffer);
    return file.println(buffer);
}

void TrackAddictCSVFormatter::writeHeader(File file, gps_fix gpsFix)
{
    // User settings:
    // U1 - Units (U1 - kph, U0 - mph)

    file.println("# RaceRender Data: EnduroGPS (Mode -1)");
    file.println("# GPS: EXT: ENDUROGPS; MODE: SD");
    file.println("# User Settings: SL1;U1;AS1;LT0/1;EC0;VC-1;VQ3;VS0;VSOIS0;VIF1");
    file.println("# Device Free Space: 56569 MB");
    file.println(R"raw("Time","UTC Time","GPS_Update","GPS_Delay","Latitude","Longitude","Altitude (m)","Altitude (ft)","Speed (Km/h)","Heading","Accuracy (m)")raw");
}
