#include "GPS/Formatter/TrackAddictCSVFormatter.h"
#include <NeoTime.h>

File TrackAddictCSVFormatter::create(gps_fix gpsFix)
{
    char csvFilePath[64];
    sprintf(csvFilePath, "/Log-20%i%02d%02d-%02d%02d%02d EnduroGPS.csv", 
        gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date, 
        gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds
    );

    File file = SD.open(csvFilePath, FILE_WRITE, true);
    writeHeader(file);
    startedAt = millis();

    return file;
}

bool TrackAddictCSVFormatter::close(File file)
{
    file.close();

    return true;
}

bool TrackAddictCSVFormatter::write(File file, gps_fix gpsFix)
{
    if (!gpsFix.valid.location) {
        log_i("Invalid location, waiting...");

        return true;
    }

    float time = (millis() - startedAt) / 1000.0;

    char buffer[2048];
    sprintf(buffer, "%.3f,%s,%i,%.3f,%.9f,%.9f,%.1f,%i,%.1f,%.1f,%.1f",
        time,                               // Time                     %.3f
        ts(gpsFix, time),                   // UTC Time                 %s
        1,                                  // GPS Update               %i
        (float) gpsFix.dateTime_cs / 1000,  // GPS Delay                %.3f
        gpsFix.latitudeL() / rescaleGPS,    // Latitude                 %.9f
        gpsFix.longitudeL() / rescaleGPS,   // Longitude                %.9f
        gpsFix.altitude(),                  // Altitude (m)             %.1f
        (uint) gpsFix.altitude_ft(),        // Altitude (ft)            %i
        gpsFix.speed_kph(),                 // Speed km/h               %.1f
        gpsFix.heading(),                   // Heading                  %.1f
        accuracy(gpsFix)                    // Accuracy (m)             %.1f
    );

    Serial.println(buffer);    
    file.println(buffer);
    file.flush();

    return true;
}

void TrackAddictCSVFormatter::writeHeader(File file)
{
    // User settings:
    // U1 - Units (U1 - kph, U0 - mph)

    file.println("# RaceRender Data: EnduroGPS (Mode -1)");
    file.println("# GPS: EXT: ENDUROGPS; MODE: BT");
    file.println("# User Settings: SL1;U1;AS1;LT0/1;EC0;VC-1;VQ3;VS0;VSOIS0;VIF1");
    file.println("# Device Free Space: 56569 MB");
    file.println(R"raw("Time","UTC Time","GPS_Update","GPS_Delay","Latitude","Longitude","Altitude (m)","Altitude (ft)","Speed (Km/h)","Heading","Accuracy (m)","Accel X","Accel Y","Accel Z","Brake (calculated)","Barometric Pressure (kPa)","Pressure Altitude (m)","Valid?")raw");
}
