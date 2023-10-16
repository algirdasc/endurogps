#include "EnduroGPS.h"
#include "Settings.h"

void Settings::save()
{
    size_t sizeWritten;
    String stringWifiMode;
    switch (storage.wifiMode) {
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

    sizeWritten = preferences.putString("wifiMode", stringWifiMode);
    sizeWritten = preferences.putString("wifiStaSsid", storage.wifiStaSsid);
    sizeWritten = preferences.putString("wifiStaPass", storage.wifiStaPass);
}

void Settings::load()
{
    preferences.begin("endurogps");

    String wifiMode = preferences.getString("wifiMode");
    if (wifiMode == "WIFI_STA") {
        storage.wifiMode = WIFI_STA;
    } else if (wifiMode == "WIFI_AP") {
        storage.wifiMode = WIFI_AP;        
    } else if (wifiMode == "WIFI_OFF") {
        storage.wifiMode = WIFI_OFF;
    } else {
        log_e("Error reading WiFi preference, defaulting to WIFI_AP");
        storage.wifiMode = WIFI_AP;
    }    

    storage.wifiStaPass = preferences.getString("wifiStaPass");
    storage.wifiStaSsid = preferences.getString("wifiStaSsid");

    log_d("wifiStaSsid: %s", storage.wifiStaSsid);
    log_d("wifiStaSsid: %s", preferences.getString("wifiStaSsid"));
}
