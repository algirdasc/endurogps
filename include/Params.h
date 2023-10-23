#pragma once

#include <Preferences.h>
#include <WiFi.h>

#define GPS_MODE_CSV 0
#define GPS_MODE_BT 1

#define LOG_FORMAT_TRACK_ADDICT_CSV 0
#define LOG_FORMAT_VOB 1

const char T_PREFERENCES_NS[] PROGMEM = "endurogps";

const char GPS_MODE[] PROGMEM = "gpsMode";
const char GPS_LOG_FORMAT[] PROGMEM = "logFormat";
const char GPS_RATE_HZ[] PROGMEM = "gpsRateHz";
const char T_GPS_BAUD_RATE[] PROGMEM = "gpsBaudRate";
const char GPS_POWER_SAVE[] PROGMEM = "gpsPowerSave";
const char GPS_NMEA_GSV[] PROGMEM = "nmeaGSV";
const char GPS_NMEA_GSA[] PROGMEM = "nmeaGSA";
const char GPS_NMEA_GBS[] PROGMEM = "nmeaGBS";
const char GPS_NMEA_GLL[] PROGMEM = "nmeaGLL";
const char GPS_NMEA_VTG[] PROGMEM = "nmeaVTG";
const char GPS_NMEA_MAIN_TALKER[] PROGMEM = "nmeaMainTalker";
const char GPS_NMEA_SV_CHANNELS[] PROGMEM = "nmeaSVChannels";
const char GPS_NMEA_TCP_ENABLED[] PROGMEM = "nmeaTcpEnabled";

const char PARAM_WIFI_AP[] PROGMEM = "WIFI_AP";
const char PARAM_WIFI_STA[] PROGMEM = "WIFI_STA";
const char PARAM_WIFI_OFF[] PROGMEM = "WIFI_OFF";

const char PARAM_WIFI_MODE[] PROGMEM = "wifiMode";
const char PARAM_WIFI_STA_SSID[] PROGMEM = "wifiStaSsid";
const char PARAM_WIFI_STA_PASS[] PROGMEM = "wifiStaPass";

struct StoredSettingsStruct
{
    WiFiMode_t wifiMode;
    String wifiStaSsid;
    String wifiStaPass;

    uint gpsMode;
    uint gpsRateHz;
    uint gpsBaudRate;
    uint gpsPowerSave;

    uint logFormat;

    bool nmeaTcpEnabled;
    bool nmeaGSA;
    bool nmeaGSV;
    bool nmeaVTG;
    bool nmeaGLL;
    bool nmeaGBS;
    uint nmeaMainTalker;
    uint nmeaSVChannels;    
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
