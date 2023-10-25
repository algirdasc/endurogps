#pragma once

#include <Preferences.h>
#include <WiFi.h>

#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/GPSPort.h"

// Default params
#define DEFAULT_PARAM_GPS_MODE          GPS_MODE_SDCARD
#define DEFAULT_PARAM_LOG_FORMAT        LOG_FORMAT_VBO
#define DEFAULT_PARAM_RATE_HZ           GPS_RATE_10_HZ
#define DEFAULT_PARAM_BAUD_RATE         GPS_RATE_115200
#define DEFAULT_PARAM_NMEA_TCP_ENABLED  false
#define DEFAULT_PARAM_WIFI_MODE         PARAM_WIFI_MODE_AP
#define DEFAULT_PARAM_WIFI_FALLBACK_AP  false

const char PARAM_PREFERENCES_NS[] PROGMEM = "endurogps";

const char PARAM_GPS_MODE[] PROGMEM = "gpsMode";
const char PARAM_GPS_LOG_FORMAT[] PROGMEM = "logFormat";
const char PARAM_GPS_RATE_HZ[] PROGMEM = "gpsRateHz";
const char PARAM_GPS_BAUD_RATE[] PROGMEM = "gpsBaudRate";
const char PARAM_GPS_NMEA_TCP_ENABLED[] PROGMEM = "nmeaTcpEnabled";

const char PARAM_WIFI_MODE_AP[] PROGMEM = "WIFI_AP";
const char PARAM_WIFI_MODE_STA[] PROGMEM = "WIFI_STA";
const char PARAM_WIFI_MODE_OFF[] PROGMEM = "WIFI_OFF";
const char PARAM_WIFI_MODE[] PROGMEM = "wifiMode";
const char PARAM_WIFI_STA_SSID[] PROGMEM = "wifiStaSsid";
const char PARAM_WIFI_STA_PASS[] PROGMEM = "wifiStaPass";
const char PARAM_WIFI_FALLBACK_AP[] PROGMEM = "wifiFallbackAp";

struct StoredSettingsStruct
{
    WiFiMode_t wifiMode;
    String wifiStaSsid;
    String wifiStaPass;
    bool wifiFallbackAp;
         
    uint gpsMode;
    uint gpsRateHz;
    uint gpsBaudRate;

    uint logFormat;

    bool nmeaTcpEnabled;
};

class Params 
{
    private:        
        Preferences preferences;                
        
    public:
        void save();        
        void load();
        String wifiMode(WiFiMode_t wifiMode);
        WiFiMode_t wifiMode(String wifiMode);
        StoredSettingsStruct storage;
};
