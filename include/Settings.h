#include <Preferences.h>
#include <WiFi.h>

#define WIFI_KEY_MAXLEN (int)64
#define WIFI_SSID_MAXLEN (int)32

struct StoredSettingsStruct
{
    WiFiMode_t wifiMode = WIFI_AP;
    char wifiStaSsid[WIFI_SSID_MAXLEN];
    char wifiStaKey[WIFI_KEY_MAXLEN];
};

class Settings 
{
    private:        
        Preferences preferences;
        StoredSettingsStruct storedSettings;
        
        void readFromMemory();

    public:
        Settings();
        
        void saveSettings();
        StoredSettingsStruct get();
};
