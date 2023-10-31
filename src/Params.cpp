#include "EnduroGPS.h"
#include "Params.h"
#include "GPS/GPSPort.h"

void Params::save()
{
    preferences.begin(PARAM_PREFERENCES_NS);

    preferences.putString(PARAM_WIFI_MODE, Params::wifiMode(storage.wifiMode));
    preferences.putString(PARAM_WIFI_STA_SSID, storage.wifiStaSsid);
    preferences.putString(PARAM_WIFI_STA_PASS, storage.wifiStaPass);
    preferences.putBool(PARAM_WIFI_FALLBACK_AP, storage.wifiFallbackAp);
    preferences.putString(PARAM_WIFI_AP_SSID, storage.wifiApSsid);
    preferences.putString(PARAM_WIFI_AP_PASS, storage.wifiApPass);

    preferences.putUInt(PARAM_GPS_MODE, storage.gpsMode);
    preferences.putUInt(PARAM_GPS_RATE_HZ, storage.gpsRateHz);
    preferences.putUInt(PARAM_GPS_BAUD_RATE, storage.gpsBaudRate);
    preferences.putBool(PARAM_GPS_NMEA_TCP_ENABLED, storage.nmeaTcpEnabled);
    preferences.putUInt(PARAM_GPS_LOG_FORMAT, storage.logFormat);
    preferences.putUInt(PARAM_GPS_OPTIMIZE_FOR, storage.gpsOptimizeFor);
    preferences.putString(PARAM_GPS_SESSION_NAME, storage.gpsSessionName);

    preferences.end();
}

void Params::load()
{
    char accessPointSSID[20];
    uint16_t chip = (uint16_t)((uint64_t)ESP.getEfuseMac() >> 32);
    sprintf(accessPointSSID, "%s-%04X", DEVICE_NAME, chip);

    preferences.begin(PARAM_PREFERENCES_NS);

    storage.wifiMode = Params::wifiMode(preferences.getString(PARAM_WIFI_MODE, DEFAULT_PARAM_WIFI_MODE));
    storage.wifiStaPass = preferences.getString(PARAM_WIFI_STA_PASS);
    storage.wifiStaSsid = preferences.getString(PARAM_WIFI_STA_SSID);
    storage.wifiFallbackAp = preferences.getBool(PARAM_WIFI_FALLBACK_AP, DEFAULT_PARAM_WIFI_FALLBACK_AP);
    storage.wifiApPass = preferences.getString(PARAM_WIFI_AP_PASS, DEFAULT_PARAM_WIFI_AP_PASS);
    storage.wifiApSsid = preferences.getString(PARAM_WIFI_AP_SSID, accessPointSSID);

    storage.logFormat = preferences.getUInt(PARAM_GPS_LOG_FORMAT, DEFAULT_PARAM_LOG_FORMAT);
    storage.gpsMode = preferences.getUInt(PARAM_GPS_MODE, DEFAULT_PARAM_GPS_MODE);
    storage.gpsRateHz = preferences.getUInt(PARAM_GPS_RATE_HZ, DEFAULT_PARAM_RATE_HZ);
    storage.gpsBaudRate = preferences.getUInt(PARAM_GPS_BAUD_RATE, DEFAULT_PARAM_BAUD_RATE);
    storage.nmeaTcpEnabled = preferences.getBool(PARAM_GPS_NMEA_TCP_ENABLED, DEFAULT_PARAM_NMEA_TCP_ENABLED);
    storage.gpsOptimizeFor = preferences.getUInt(PARAM_GPS_OPTIMIZE_FOR, DEFAULT_PARAM_GPS_OPTIMIZE_FOR);
    storage.gpsSessionName = preferences.getString(PARAM_GPS_SESSION_NAME, DEFAULT_PARAM_GPS_SESSION_NAME);

    preferences.end();
}

String Params::wifiMode(WiFiMode_t wifiMode)
{
    String stringWifiMode;
    switch (wifiMode)
    {
    case WIFI_AP:
        stringWifiMode = PARAM_WIFI_MODE_AP;
        break;
    case WIFI_STA:
        stringWifiMode = PARAM_WIFI_MODE_STA;
        break;
    case WIFI_OFF:
        stringWifiMode = PARAM_WIFI_MODE_OFF;
        break;
    default:
        stringWifiMode = PARAM_WIFI_MODE_AP;
        break;
    }

    return stringWifiMode;
}

WiFiMode_t Params::wifiMode(String wifiMode)
{
    if (wifiMode.equals(PARAM_WIFI_MODE_STA))
    {
        return WIFI_STA;
    }
    else if (wifiMode.equals(PARAM_WIFI_MODE_AP))
    {
        return WIFI_AP;
    }
    else if (wifiMode.equals(PARAM_WIFI_MODE_OFF))
    {
        return WIFI_OFF;
    }

    return WIFI_AP;
}
