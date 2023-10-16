#include <WiFi.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>

#include "EnduroGPS.h"
#include "WifiMode.h"

WifiMode::WifiMode() {
  uint16_t chip = (uint16_t)((uint64_t) ESP.getEfuseMac() >> 32);
  sprintf(accessPointSSID, "%s-%04X", ENDUROGPS_AP_SSID, chip);

  WiFi.setHostname(ENDUROGPS_AP_MDNS);
}

void WifiMode::start(WiFiMode_t mode)
{
    if (mode == WIFI_STA) {        
        WifiMode::STA();
        log_i("Started WiFi in STA mode");
    } else if (mode == WIFI_AP) {
        WifiMode::AP();
        log_i("Started WiFi in AP mode");
    } else if (mode == WIFI_OFF) {
        WifiMode::OFF();
        log_i("Started WiFi in OFF mode");
    } else {
        WifiMode::AP();
        log_i("Started WiFi in AP mode (fallback)");
    }
}

void WifiMode::AP() 
{
    WiFi.mode(WIFI_AP);

    wifi_country_t country {
        .cc = "LT",
        .schan = 1,
        .nchan = 13,
        .max_tx_power = 20,
        .policy = WIFI_COUNTRY_POLICY_AUTO,
    };

    esp_wifi_set_country(&country);

    WiFi.softAP(accessPointSSID, ENDUROGPS_AP_PASS, 1, 0, 2);
    log_i("Wait 100 ms for AP_START...");
    delay(100);
    log_d("Power: %d", WiFi.getTxPower());

    IPAddress ip(10, 0, 0, 1);
    IPAddress netMask(255, 255, 255, 0);

    WiFi.softAPConfig(ip, ip, netMask);

    IPAddress currentIP = WiFi.softAPIP();
    log_i("AP IP address: %s", currentIP.toString());

    // if (!MDNS.begin(ENDUROGPS_AP_MDNS)) {
    //     log_e("Error setting up MDNS responder!");
    // } else {
    //     log_d("Started MDNS");
    // }

    isConnected = true;
}

void WifiMode::STA() 
{
    WiFi.mode(WIFI_STA);
    
    WiFi.begin();

    int times = 0;
    while (WiFi.status() != WL_CONNECTED && times < 15) {
        delay(1000);
        log_i("Connecting to WiFi");
    }

    if (WiFi.status() == WL_CONNECTED) {

    } else {
        // log_e("Could not connect to SSID %s, reverting to AP mode", stored_preferences.wifi_ssid);
        WifiMode::AP();
    }
}

void WifiMode::OFF() 
{
    WiFi.mode(WIFI_OFF);

    MDNS.end();

    isConnected = false;
}
