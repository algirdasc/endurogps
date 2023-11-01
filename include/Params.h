#pragma once

#include <Preferences.h>
#include <WiFi.h>

#include "GPS/Formatter/BaseFormatter.h"
#include "GPS/GPSPort.h"

// Default params
#define DEFAULT_PARAM_GPS_MODE GPS_MODE_SDCARD
#define DEFAULT_PARAM_LOG_FORMAT LOG_FORMAT_VBO
#define DEFAULT_PARAM_RATE_HZ GPS_RATE_10_HZ
#define DEFAULT_PARAM_NMEA_TCP_ENABLED false
#define DEFAULT_PARAM_WIFI_MODE PARAM_WIFI_MODE_AP
#define DEFAULT_PARAM_WIFI_FALLBACK_AP false
#define DEFAULT_PARAM_WIFI_AP_NAME ""
#define DEFAULT_PARAM_WIFI_AP_PASS "endurogps815"
#define DEFAULT_PARAM_GPS_OPTIMIZE_FOR GPS_OPTIMIZE_FOR_DEFAULT
#define DEFAULT_PARAM_GPS_SESSION_NAME DEVICE_NAME

const char PARAM_PREFERENCES_NS[] = "endurogps";

const char PARAM_GPS_MODE[] = "gpsMode";
const char PARAM_GPS_LOG_FORMAT[] = "logFormat";
const char PARAM_GPS_RATE_HZ[] = "gpsRateHz";
const char PARAM_GPS_NMEA_TCP_ENABLED[] = "nmeaTcpEnabled";
const char PARAM_GPS_OPTIMIZE_FOR[] = "gpsOptimizeFor";
const char PARAM_GPS_SESSION_NAME[] = "gpsSessionName";
const char PARAM_WIFI_MODE_AP[] = "WIFI_AP";
const char PARAM_WIFI_MODE_STA[] = "WIFI_STA";
const char PARAM_WIFI_MODE_OFF[] = "WIFI_OFF";
const char PARAM_WIFI_MODE[] = "wifiMode";
const char PARAM_WIFI_AP_SSID[] = "wifiApSsid";
const char PARAM_WIFI_AP_PASS[] = "wifiApPass";
const char PARAM_WIFI_STA_SSID[] = "wifiStaSsid";
const char PARAM_WIFI_STA_PASS[] = "wifiStaPass";
const char PARAM_WIFI_FALLBACK_AP[] = "wifiFallbackAp";
const char PARAM_TRACK_L1_LATITUDE[] = "l1Lat";
const char PARAM_TRACK_L1_LONGITUDE[] = "l1Lon";
const char PARAM_TRACK_L2_LATITUDE[] = "l2Lat";
const char PARAM_TRACK_L2_LONGITUDE[] = "l2Lon";

struct StoredSettingsStruct
{
    WiFiMode_t wifiMode;
    String wifiStaSsid;
    String wifiStaPass;
    String wifiApPass;
    String wifiApSsid;
    bool wifiFallbackAp;

    uint gpsMode;
    uint gpsRateHz;
    uint gpsOptimizeFor;
    uint logFormat;
    String gpsSessionName;

    String l1Lat;
    String l1Lon;
    String l2Lat;
    String l2Lon;

    bool nmeaTcpEnabled;
};

class Params
{
private:
    Preferences preferences;

public:
    void save();
    void saveString(const char *paramName);
    void load();
    String wifiMode(WiFiMode_t wifiMode);
    WiFiMode_t wifiMode(String wifiMode);
    StoredSettingsStruct storage;
};
