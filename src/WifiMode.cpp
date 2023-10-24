#include <WiFi.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>

#include "EnduroGPS.h"
#include "WifiMode.h"

WifiMode::WifiMode() {
  uint16_t chip = (uint16_t)((uint64_t) ESP.getEfuseMac() >> 32);
  sprintf(accessPointSSID, "%s-%04X", DEVICE_NAME, chip);

  WiFi.setHostname(WIFI_AP_MDNS);
}

void WifiMode::setMode(WiFiMode_t mode)
{
    if (mode == WIFI_STA) {                
        WifiMode::STA();
    } else if (mode == WIFI_AP) {
        WifiMode::AP();
    } else if (mode == WIFI_OFF) {
        WifiMode::OFF();
    } else {
        WifiMode::AP();
    }
}

void WifiMode::AP() 
{
    currentMode = WIFI_AP;

    WiFi.mode(WIFI_AP);

    wifi_country_t country {
        .cc = "LT",
        .schan = 1,
        .nchan = 13,
        .max_tx_power = 20,
        .policy = WIFI_COUNTRY_POLICY_AUTO,
    };

    esp_wifi_set_country(&country);

    WiFi.softAP(accessPointSSID, WIFI_AP_PASS, 1, 0, 2);
    log_i("Wait 100 ms for AP_START...");
    delay(100);
    log_d("Power: %d", WiFi.getTxPower());

    IPAddress ip(10, 0, 0, 1);
    IPAddress netMask(255, 255, 255, 0);

    WiFi.softAPConfig(ip, ip, netMask);

    IPAddress currentIP = WiFi.softAPIP();
}

void WifiMode::STA() 
{
    currentMode = WIFI_STA;

    WiFi.mode(WIFI_STA);
    WiFi.begin(staSsid, staPass);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 15) {
        attempt++;
        log_d("Waiting for WiFi, attempt #%d...", attempt);
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED) {
        if (fallbackToAP) {
            log_d("Fallbacking to AP mode");
            WifiMode::AP();
        } else {
            log_d("Failed to connect, turning WiFi off");
            WifiMode::OFF();
        }
    }
}

void WifiMode::OFF() 
{
    currentMode = WIFI_OFF;
    WiFi.mode(WIFI_OFF);
}
