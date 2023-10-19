#include <WiFi.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>

#include "EnduroGPS.h"
#include "WifiMode.h"

WifiMode::WifiMode() {
  uint16_t chip = (uint16_t)((uint64_t) ESP.getEfuseMac() >> 32);
  sprintf(accessPointSSID, "%s-%04X", WIFI_AP_SSID, chip);

  WiFi.setHostname(WIFI_AP_MDNS);
}

void WifiMode::mode(WiFiMode_t mode)
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

    isConnected = true;
}

void WifiMode::STA() 
{
    currentMode = WIFI_STA;

    WiFi.mode(WIFI_STA);
    WiFi.begin(STASsid, STAKey);
    log_d("Connecting to %s", STASsid);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 15) {
        log_d("Waiting for WiFi, try #%d...", tries);
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED) {
        log_d("Fallbacking to AP mode");
        WifiMode::AP();
    }
}

void WifiMode::OFF() 
{
    currentMode = WIFI_OFF;

    WiFi.mode(WIFI_OFF);

    isConnected = false;
}

void WifiMode::setSTAcredentials(String STASsid, String STAKey)
{
    WifiMode::STASsid = STASsid;
    WifiMode::STAKey = STAKey;
}
