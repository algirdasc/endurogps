#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>
#include <WiFi.h>

#define GPS_MODE_CSV 0
#define GPS_MODE_BLE 1

#define LOG_FORMAT_TRACK_ADDICT_CSV 0
#define LOG_FORMAT_VOB 1

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

#endif
