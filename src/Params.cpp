#include "EnduroGPS.h"
#include "Params.h"
#include "GPS/GPSPort.h"

void Params::save()
{
    preferences.begin(T_PREFERENCES_NS);

    preferences.putString("wifiMode", Params::wifiMode(storage.wifiMode));
    preferences.putString("wifiStaSsid", storage.wifiStaSsid);
    preferences.putString("wifiStaPass", storage.wifiStaPass);    

    preferences.putUInt(GPS_MODE, storage.gpsMode);
    preferences.putUInt(GPS_RATE_HZ, storage.gpsRateHz);
    preferences.putUInt(T_GPS_BAUD_RATE, storage.gpsBaudRate);
    preferences.putUInt(GPS_POWER_SAVE, storage.gpsPowerSave);

    preferences.putBool(GPS_NMEA_TCP_ENABLED, storage.nmeaTcpEnabled);
    preferences.putBool(GPS_NMEA_GSV, storage.nmeaGSV);
    preferences.putBool(GPS_NMEA_GSA, storage.nmeaGSA);
    preferences.putBool(GPS_NMEA_GBS, storage.nmeaGBS);
    preferences.putBool(GPS_NMEA_GLL, storage.nmeaGLL);
    preferences.putBool(GPS_NMEA_VTG, storage.nmeaVTG);
    preferences.putUInt(GPS_NMEA_MAIN_TALKER, storage.nmeaMainTalker);
    preferences.putUInt(GPS_NMEA_SV_CHANNELS, storage.nmeaSVChannels);

    preferences.putUInt(GPS_LOG_FORMAT, storage.logFormat);

    preferences.end();
}

void Params::load()
{
    preferences.begin(T_PREFERENCES_NS);

    storage.wifiMode = Params::wifiMode(preferences.getString("wifiMode"));
    storage.wifiStaPass = preferences.getString("wifiStaPass");
    storage.wifiStaSsid = preferences.getString("wifiStaSsid");

    storage.logFormat = preferences.getUInt(GPS_LOG_FORMAT, LOG_FORMAT_TRACK_ADDICT_CSV);

    storage.gpsMode = preferences.getUInt(GPS_MODE, GPS_MODE_CSV);
    storage.gpsRateHz = preferences.getUInt(GPS_RATE_HZ, 10);
    storage.gpsBaudRate = preferences.getUInt(T_GPS_BAUD_RATE, GPS_BAUD_RATE);
    storage.gpsPowerSave = preferences.getUInt(GPS_POWER_SAVE, GPSPORT_POWER_SAVE);

    storage.nmeaTcpEnabled = preferences.getBool(GPS_NMEA_TCP_ENABLED, false);
    storage.nmeaGSV = preferences.getBool(GPS_NMEA_GSV, false);
    storage.nmeaGSA = preferences.getBool(GPS_NMEA_GSA, false);
    storage.nmeaGBS = preferences.getBool(GPS_NMEA_GBS, false);
    storage.nmeaGLL = preferences.getBool(GPS_NMEA_GLL, false);
    storage.nmeaVTG = preferences.getBool(GPS_NMEA_VTG, false);
    storage.nmeaMainTalker = preferences.getUInt(GPS_NMEA_MAIN_TALKER, GPSPORT_MAINTALKER_GP);
    storage.nmeaSVChannels = preferences.getUInt(GPS_NMEA_SV_CHANNELS, GPSPORT_SV_CHANNELS);

    preferences.end();
}

String Params::wifiMode(WiFiMode_t wifiMode)
{
    String stringWifiMode;
    switch (wifiMode) {
        case WIFI_AP:
            stringWifiMode = PARAM_WIFI_AP;
            break;
        case WIFI_STA:
            stringWifiMode = PARAM_WIFI_STA;
            break;
        case WIFI_OFF:
            stringWifiMode = PARAM_WIFI_OFF;
            break;
        default:
            stringWifiMode = PARAM_WIFI_AP;
            break;
    }

    return stringWifiMode;
}

WiFiMode_t Params::wifiMode(String wifiMode)
{
    // TODO: string comparison with "equals"
    if (wifiMode == PARAM_WIFI_STA) {
        return WIFI_STA;
    } else if (wifiMode == PARAM_WIFI_AP) {
        return WIFI_AP;        
    } else if (wifiMode == PARAM_WIFI_OFF) {
        return WIFI_OFF;
    }
    
    return WIFI_AP;
}
