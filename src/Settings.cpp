#include "EnduroGPS.h"
#include "Settings.h"

Settings::Settings() 
{
    preferences.begin(ENDUROGPS_AP_MDNS);
    Settings::readFromMemory();
}

StoredSettingsStruct Settings::get() 
{
    return storedSettings;
}

void Settings::saveSettings()
{
    String stringWifiMode;
    switch (storedSettings.wifiMode) {
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

    preferences.putString("wifiMode", stringWifiMode);
}

void Settings::readFromMemory()
{
    String wifiMode = preferences.getString("wifiMode");
    if (wifiMode == "WIFI_STA") {
        storedSettings.wifiMode = WIFI_STA;
    } else if (wifiMode == "WIFI_AP") {
        storedSettings.wifiMode = WIFI_AP;        
    } else if (wifiMode == "WIFI_OFF") {
        storedSettings.wifiMode = WIFI_OFF;
    } else {
        log_e("Error reading WiFi preference, defaulting to WIFI_AP");
        storedSettings.wifiMode = WIFI_AP;
    }
}