#include "EnduroGPS.h"
#include "Params.h"
#include "GPS/GPSPort.h"

void Params::save()
{
    preferences.begin("endurogps");

    preferences.putString("wifiMode", Params::wifiMode(storage.wifiMode));
    preferences.putString("wifiStaSsid", storage.wifiStaSsid);
    preferences.putString("wifiStaPass", storage.wifiStaPass);    

    preferences.putUInt("gpsMode", storage.gpsMode);
    preferences.putUInt("gpsRateHz", storage.gpsRateHz);
    preferences.putUInt("gpsBaudRate", storage.gpsBaudRate);
    preferences.putUInt("gpsPowerSave", storage.gpsPowerSave);

    preferences.putBool("nmeaTcpEnabled", storage.nmeaTcpEnabled);
    preferences.putBool("nmeaGSV", storage.nmeaGSV);
    preferences.putBool("nmeaGSA", storage.nmeaGSA);
    preferences.putBool("nmeaGBS", storage.nmeaGBS);
    preferences.putBool("nmeaGLL", storage.nmeaGLL);
    preferences.putBool("nmeaVTG", storage.nmeaVTG);
    preferences.putUInt("nmeaMainTalker", storage.nmeaMainTalker);
    preferences.putUInt("nmeaSVChannels", storage.nmeaSVChannels);

    preferences.putUInt("logFormat", storage.logFormat);

    preferences.end();
}

void Params::load()
{
    preferences.begin("endurogps");

    storage.wifiMode = Params::wifiMode(preferences.getString("wifiMode"));
    storage.wifiStaPass = preferences.getString("wifiStaPass");
    storage.wifiStaSsid = preferences.getString("wifiStaSsid");

    storage.logFormat = preferences.getUInt("logFormat", LOG_FORMAT_TRACK_ADDICT_CSV);

    storage.gpsMode = preferences.getUInt("gpsMode", GPS_MODE_CSV);
    storage.gpsRateHz = preferences.getUInt("gpsRateHz", 10);
    storage.gpsBaudRate = preferences.getUInt("gpsBaudRate", GPS_STANDARD_BAUD_RATE);
    storage.gpsPowerSave = preferences.getUInt("gpsPowerSave", GPSPORT_POWER_SAVE);

    storage.nmeaTcpEnabled = preferences.getBool("nmeaTcpEnabled", false);
    storage.nmeaGSV = preferences.getBool("nmeaGSV", false);
    storage.nmeaGSA = preferences.getBool("nmeaGSA", false);
    storage.nmeaGBS = preferences.getBool("nmeaGBS", false);
    storage.nmeaGLL = preferences.getBool("nmeaGLL", false);
    storage.nmeaVTG = preferences.getBool("nmeaVTG", false);
    storage.nmeaMainTalker = preferences.getUInt("nmeaMainTalker", GPSPORT_MAINTALKER_GP);
    storage.nmeaSVChannels = preferences.getUInt("nmeaSVChannels", GPSPORT_SV_CHANNELS);

    preferences.end();
}

String Params::wifiMode(WiFiMode_t wifiMode)
{
    String stringWifiMode;
    switch (wifiMode) {
        case WIFI_AP:
            stringWifiMode = "WIFI_AP";
            break;
        case WIFI_STA:
            stringWifiMode = "WIFI_STA";
            break;
        case WIFI_OFF:
            stringWifiMode = "WIFI_OFF";
            break;
        default:
            stringWifiMode = "WIFI_AP";
            break;
    }

    return stringWifiMode;
}

WiFiMode_t Params::wifiMode(String wifiMode)
{
    if (wifiMode == "WIFI_STA") {
        return WIFI_STA;
    } else if (wifiMode == "WIFI_AP") {
        return WIFI_AP;        
    } else if (wifiMode == "WIFI_OFF") {
        return WIFI_OFF;
    }
    
    return WIFI_AP;
}
