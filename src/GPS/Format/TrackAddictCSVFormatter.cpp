#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include <NeoTime.h>

const float rescaleGPS = 10000000.0;

File TrackAddictCSVFormatter::create(gps_fix gpsFix)
{
    char csvFilePath[64];
    sprintf(csvFilePath, "/Log-20%i%02d%02d-%02d%02d%02d EnduroGPS.csv", 
        gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date, 
        gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds
    );

    File file = SD.open(csvFilePath, FILE_APPEND, true);
    writeHeader(file);

    createdAt = millis();

    return file;
}

bool TrackAddictCSVFormatter::close(File file)
{
    file.close();

    return 1;
}

bool TrackAddictCSVFormatter::write(File file, gps_fix gpsFix)
{
    float time = (millis() - createdAt) / 1000.0;

    char buffer[2048];
    sprintf(buffer, "%.3f,%.3f,%i,%.3f,%.9f,%.9f,%.1f,%i,%.1f,%.1f,%.1f,%.2f,%.2f,%.2f,%i,%.2f,%.1f",
        time,                               // Time                     %.3f
        (float) ((clock_t) gpsFix.dateTime),// UTC Time                 %.3f
        1,                                  // GPS Update               %i
        (float) gpsFix.dateTime_cs / 1000,  // GPS Delay                %.3f
        gpsFix.latitudeL() / rescaleGPS,    // Latitude                 %.8f
        gpsFix.longitudeL() / rescaleGPS,   // Longitude                %.8f
        gpsFix.altitude(),                  // Altitude (m)             %.1f
        (uint) gpsFix.altitude_ft(),        // Altitude (ft)            %i
        gpsFix.speed_kph(),                 // Speed km/h               %.1f
        gpsFix.heading(),                   // Heading                  %.1f
        1.0,                               // Accuracy (m)             %.1f
        0.1,                               // Accel X                  %.2f
        0.2,                               // Accel Y                  %.2f
        0.3,                               // Accel Z                  %.2f
        0,                                 // Brake (calculated)       %i
        0.0,                               // Barometric pressure      %.2f
        0.0                                // Pressure altitude (m)    %.1f
    );

    file.println(buffer);    
    file.flush();

    return 1;
}

void TrackAddictCSVFormatter::writeHeader(File file)
{
    file.println("# RaceRender Data: EnduroGPS (Mode -1)");
    file.println("# GPS: EXT: ENDUROGPS; MODE: BT");
    file.println("# User Settings: SL1;U1;AS1;LT0/1;EC0;VC-1;VQ3;VS0;VSOIS0;VIF1");
    file.println("# Device Free Space: 56569 MB");
    file.println(R"raw("Time","UTC Time","GPS_Update","GPS_Delay","Latitude","Longitude","Altitude (m)","Altitude (ft)","Speed (Km/h)","Heading","Accuracy (m)","Accel X","Accel Y","Accel Z","Brake (calculated)","Barometric Pressure (kPa)","Pressure Altitude (m)")raw");
}
