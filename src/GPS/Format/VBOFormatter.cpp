#include "EnduroGPS.h"
#include "GPS/Formatter/VBOFormatter.h"

void VOBFormatter::filepath(char *filePath, gps_fix gpsFix)
{
    sprintf(filePath, "/Log-20%02d%02d%02d-%02d%02d%02d_%s.vbo",
            gpsFix.dateTime.year, gpsFix.dateTime.month, gpsFix.dateTime.date,
            gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds,
            gpsSessionName);
}

size_t VOBFormatter::write(File file, gps_fix gpsFix)
{
    ulong currentMillis = millis();
    ulong rateCalulationDiff = currentMillis - lastRateCalculationAt;
    if (rateCalulationDiff > 1000)
    {
        updateRate = writeCount / (float)rateCalulationDiff * 1000;
        lastRateCalculationAt = currentMillis;
        writeCount = 0;
    }

    char buffer[2048];
    snprintf(buffer, 2048,
             "%03d %02d%02d%02d.%03d %s%05d.%02d%03d %s%05d.%02d%03d %07.03f %06.02f %08.02f %07.03f %05.02f %05.02f",
             gpsFix.satellites,                                                // Satellites            %03d
             gpsFix.dateTime.hours,                                            // Hours                 %02d
             gpsFix.dateTime.minutes,                                          // Minutes               %02d
             gpsFix.dateTime.seconds,                                          // Seconds               %02d
             gpsFix.dateTime_ms(),                                             // Miliseconds           %02d
             gpsFix.latitudeDMS.hemisphere == NORTH_H ? "+" : "-",             // Plus / Minus          %s
             (gpsFix.latitudeDMS.degrees * 60) + gpsFix.latitudeDMS.minutes,   // Latitude              %05d
             gpsFix.latitudeDMS.seconds_whole,                                 // Latitude              %02d
             gpsFix.latitudeDMS.seconds_frac,                                  // Latitude              %03d
             gpsFix.longitudeDMS.hemisphere == WEST_H ? "+" : "-",             // Plus / Minus          %s
             (gpsFix.longitudeDMS.degrees * 60) + gpsFix.longitudeDMS.minutes, // Longitude             %05d
             gpsFix.longitudeDMS.seconds_whole,                                // Longitude             %02d
             gpsFix.longitudeDMS.seconds_frac,                                 // Longitude             %03d
             gpsFix.speed_kph(),                                               // Speed                 %07.03f
             gpsFix.heading(),                                                 // Heading               %06.02f
             gpsFix.altitude(),                                                // Altitude              %08.02f
             updateRate,                                                       // Device upd rate       %07.03f
             latLonAccuracy(gpsFix),                                           // coordinate_precision  %05.02f
             gpsFix.alt_err()                                                  // altitude_precision    %05.02f
    );

    writeCount++;   
    return file.println(buffer);
}

void VOBFormatter::writeHeader(File file, gps_fix gpsFix)
{
    char title[256];
    snprintf(title, 256,
             "File created on %02d/%02d/20%02d at %02d:%02d:%02d",
             gpsFix.dateTime.date, gpsFix.dateTime.month, gpsFix.dateTime.year,
             gpsFix.dateTime.hours, gpsFix.dateTime.minutes, gpsFix.dateTime.seconds);

    file.println(title);
    file.println();
    file.println("[header]");
    file.println("satellites");
    file.println("time");
    file.println("latitude");
    file.println("longitude");
    file.println("velocity kmh");
    file.println("heading");
    file.println("height");
    file.println("device_update_rate");
    file.println("coordinate_precision");
    file.println("altitude_precision");
    file.println("");
    file.println("[comments]");
    file.println("Generated by " DEVICE_NAME);
    file.println("");
    file.println("[session data]");
    file.print("name ");
    file.print(gpsSessionName);
    file.println("");
    file.println("");
    file.println("[column names]");
    file.println("sats time lat long velocity heading height device_update_rate coordinate_precision altitude_precision");
    file.println("");
    file.println("[data]");
}
