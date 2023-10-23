#include "GPS/Formatter/VBOFormatter.h"

File VOBFormatter::create(gps_fix gpsFix)
{
    char csvFilePath[64];
    sprintf(csvFilePath, "/Log-20%i%02d%02d-%02d%02d%02d EnduroGPS.vbo", 
        gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date, 
        gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds
    );

    File file = SD.open(csvFilePath, FILE_WRITE, true);
    writeHeader(file);
    startedAt = millis();

    return file;
}

bool VOBFormatter::write(File file, gps_fix gpsFix)
{
    if (!gpsFix.valid.location) {
        log_i("Invalid location, waiting...");

        return true;
    }

    float time = (millis() - startedAt) / 1000.0;

    char buffer[2048];
    sprintf(buffer, "%i %s %.9f %.9f %.1f %.1f %.1f %.1f",
        gpsFix.satellites,                      // Satellites   %i
        ts(gpsFix, time),                       // UTC Time     %s
        gpsFix.latitudeL() / rescaleGPS,        // Latitude     %.9f
        gpsFix.longitudeL() / rescaleGPS,       // Longitude    %.9f
        gpsFix.speed_kph(),                     // Speed        %.1f
        gpsFix.heading(),                       // Heading      %.1f
        gpsFix.altitude(),                      // Altitude     %.1f
        accuracy(gpsFix)                        // Accuracy     %.1f
    );

    Serial.println(buffer);
    file.println(buffer);
    file.flush();

    return true;
}

bool VOBFormatter::close(File file)
{
    file.close();

    return true;
}

void VOBFormatter::writeHeader(File file)
{
    file.println(F("File created on 31/07/2006 at 09:55:20 'generated from the VBOX internal Real time clock'"));
    file.println();    
    file.println("[header]");
    file.println("Satellites");
    file.println("Time");
    file.println("Latitude");
    file.println("Longitude");
    file.println("Velocity (kph)");
    file.println("Heading");
    file.println("Altitude (m)");
    file.println("Accuracy (m)");
    file.println("");
    file.println("[comments]");
    file.println("Log rate: ");
    file.println("");    
    file.println("[column names]");
    file.println("sats time lat long velocity heading PressureAltitude(m) accuracy");
    file.println("");
    file.println("[data]");
}