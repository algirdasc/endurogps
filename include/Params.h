#pragma once

#include <Preferences.h>
#include <WiFi.h>

#define GPS_MODE_CSV 0
#define GPS_MODE_BT 1

#define LOG_FORMAT_TRACK_ADDICT_CSV 0
#define LOG_FORMAT_VOB 1

const char PARAM_PREFERENCES_NS[] PROGMEM = "endurogps";

const char PARAM_GPS_MODE[] PROGMEM = "gpsMode";
const char PARAM_GPS_LOG_FORMAT[] PROGMEM = "logFormat";
const char PARAM_GPS_RATE_HZ[] PROGMEM = "gpsRateHz";
const char PARAM_GPS_BAUD_RATE[] PROGMEM = "gpsBaudRate";
const char PARAM_GPS_NMEA_GSV[] PROGMEM = "nmeaGSV";
const char PARAM_GPS_NMEA_GSA[] PROGMEM = "nmeaGSA";
const char PARAM_GPS_NMEA_GBS[] PROGMEM = "nmeaGBS";
const char PARAM_GPS_NMEA_GLL[] PROGMEM = "nmeaGLL";
const char PARAM_GPS_NMEA_VTG[] PROGMEM = "nmeaVTG";
const char PARAM_GPS_NMEA_MAIN_TALKER[] PROGMEM = "nmeaMainTalker";
const char PARAM_GPS_NMEA_SV_CHANNELS[] PROGMEM = "nmeaSVChannels";
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
