#include "EnduroGPS.h"
#include "Params.h"
#include "GPS/GPSPort.h"

void Params::save()
{
    preferences.begin(PARAM_PREFERENCES_NS);

    preferences.putString(PARAM_WIFI_MODE, Params::wifiMode(storage.wifiMode));
    preferences.putString(PARAM_WIFI_STA_SSID, storage.wifiStaSsid);
    preferences.putString(PARAM_WIFI_STA_PASS, storage.wifiStaPass);    

    preferences.putUInt(PARAM_GPS_MODE, storage.gpsMode);
    preferences.putUInt(PARAM_GPS_RATE_HZ, storage.gpsRateHz);
    preferences.putUInt(PARAM_GPS_BAUD_RATE, storage.gpsBaudRate);

    preferences.putBool(PARAM_GPS_NMEA_TCP_ENABLED, storage.nmeaTcpEnabled);
    preferences.putBool(PARAM_GPS_NMEA_GSV, storage.nmeaGSV);
    preferences.putBool(PARAM_GPS_NMEA_GSA, storage.nmeaGSA);
    preferences.putBool(PARAM_GPS_NMEA_GBS, storage.nmeaGBS);
    preferences.putBool(PARAM_GPS_NMEA_GLL, storage.nmeaGLL);
    preferences.putBool(PARAM_GPS_NMEA_VTG, storage.nmeaVTG);
    preferences.putUInt(PARAM_GPS_NMEA_MAIN_TALKER, storage.nmeaMainTalker);
    preferences.putUInt(PARAM_GPS_NMEA_SV_CHANNELS, storage.nmeaSVChannels);

    preferences.putUInt(PARAM_GPS_LOG_FORMAT, storage.logFormat);

    preferences.end();
}

void Params::load()
{
    preferences.begin(PARAM_PREFERENCES_NS);

    storage.wifiMode = Params::wifiMode(preferences.getString(PARAM_WIFI_MODE));
    storage.wifiStaPass = preferences.getString(PARAM_WIFI_STA_PASS);
    storage.wifiStaSsid = preferences.getString(PARAM_WIFI_STA_SSID);

    storage.logFormat = preferences.getUInt(PARAM_GPS_LOG_FORMAT, LOG_FORMAT_TRACK_ADDICT_CSV);

    storage.gpsMode = preferences.getUInt(PARAM_GPS_MODE, GPS_MODE_CSV);
    storage.gpsRateHz = preferences.getUInt(PARAM_GPS_RATE_HZ, 10);
    storage.gpsBaudRate = preferences.getUInt(PARAM_GPS_BAUD_RATE, GPS_BAUD_RATE);

    storage.nmeaTcpEnabled = preferences.getBool(PARAM_GPS_NMEA_TCP_ENABLED, false);
    storage.nmeaGSV = preferences.getBool(PARAM_GPS_NMEA_GSV, false);
    storage.nmeaGSA = preferences.getBool(PARAM_GPS_NMEA_GSA, false);
    storage.nmeaGBS = preferences.getBool(PARAM_GPS_NMEA_GBS, false);
    storage.nmeaGLL = preferences.getBool(PARAM_GPS_NMEA_GLL, false);
    storage.nmeaVTG = preferences.getBool(PARAM_GPS_NMEA_VTG, false);
    storage.nmeaMainTalker = preferences.getUInt(PARAM_GPS_NMEA_MAIN_TALKER, GPSPORT_MAINTALKER_GP);
    storage.nmeaSVChannels = preferences.getUInt(PARAM_GPS_NMEA_SV_CHANNELS, GPSPORT_SV_CHANNELS);

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
