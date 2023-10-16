#include <Preferences.h>
#include <WiFi.h>

#define GPS_MODE_CSV 0
#define GPS_MODE_PROXY 1

struct StoredSettingsStruct
{
    WiFiMode_t wifiMode = WIFI_AP;
    String wifiStaSsid;
    String wifiStaPass;
    int gpsMode;
};

class Settings 
{
    private:        
        Preferences preferences;                
        
    public:
        void save();        
        void load();
        StoredSettingsStruct storage;
};
