#include "GPS/Formatter/TrackAddictCSVFormatter.h"

File TrackAddictCSVFormatter::create()
{
    File file = SD.open("/test.txt", FILE_APPEND);
    writeHeader(file);

    return file;
}

bool TrackAddictCSVFormatter::write(File file, gps_fix gpsFix)
{
    file.print(",");    
    file.print(gpsFix.dateTime_us());
    file.print(",");
    file.print(gpsFix.latitude());
    file.print(",");
    file.print(gpsFix.longitude());
    file.print(",");
    file.print(gpsFix.altitude_cm() * 100);
    file.print(",");
    file.print(gpsFix.altitude_ft());
    file.print(",");
    file.print(gpsFix.speed_kph());
    file.println();
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
