#include "GPS/Formatter/VBOFormatter.h"

const char VBO_FILE_FORMAT[] = "/Log-20%i%02d%02d-%02d%02d%02d.vbo";
const char VBO_DATA_FORMAT[] = "%i %s %.9f %.9f %.1f %.1f %.1f %.1f";

void VOBFormatter::filepath(char *filePath, gps_fix gpsFix)
{
    sprintf(filePath, VBO_FILE_FORMAT, 
        gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date, 
        gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds
    );
}

bool VOBFormatter::write(File file, gps_fix gpsFix)
{
    char buffer[2048];
    sprintf(buffer, VBO_DATA_FORMAT,
        gpsFix.satellites,                      // Satellites   %i
        ts(gpsFix, VOBFormatter::duration()),   // UTC Time     %s
        gpsFix.latitudeL() / RESCALE_GPS,       // Latitude     %.9f
        gpsFix.longitudeL() / RESCALE_GPS,      // Longitude    %.9f
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

void VOBFormatter::writeHeader(File file, gps_fix gpxFix)
{
    file.println("File created on 31/07/2006 at 09:55:20 'generated from the VBOX internal Real time clock'");
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
    file.println("sats time lat long velocity heading altitude(m) accuracy(m)");
    file.println("");
    file.println("[data]");
}