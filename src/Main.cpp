#include "Main.h"
#include <WiFiServer.h>

bool isRecording = false;

Scheduler ts;
WifiMode wifiMode;
EasyButton button(GPIO_BUTTON);
LED statusLed(LED_BUILTIN);
SDCard sdCard;
NMEAServer nmeaServer;
GPSPort gpsPort;
Params params;
Battery battery;
HTTP http;
GPSBLEProxy gpsBLEProxy;
GPSLogProxy gpsLogProxy;
WiFiServer testServer(11188);

void ledBlink()
{
    statusLed.toggle();
}
Task taskStatusLedBlink(0, TASK_FOREVER, &ledBlink, &ts, false);

void gpsInitialize()
{    
    gpsPort.initialize();
    gpsPort.setRate(params.storage.gpsRateHz);
    gpsPort.setGSV(params.storage.nmeaGSV);
    gpsPort.setGSA(params.storage.nmeaGSA);
    gpsPort.setGBS(params.storage.nmeaGBS);
    gpsPort.setGLL(params.storage.nmeaGLL);
    gpsPort.setVTG(params.storage.nmeaVTG);
    gpsPort.setMainTalker(params.storage.nmeaMainTalker);
    gpsPort.setSVChannels(params.storage.nmeaSVChannels);
    gpsPort.setPowerSave(params.storage.gpsPowerSave);
}

void startRecording()
{
    gpsInitialize();    

    switch (params.storage.gpsMode) {
        case GPS_MODE_BLE:
            gpsBLEProxy.start();
            break;
        case GPS_MODE_CSV:        
            gpsLogProxy.formatter(params.storage.logFormat);
            gpsLogProxy.start();
            break;
    }

    isRecording = true;
}

void stopRecording()
{
    // gpsBLEProxy.stop();
    // gpsLogProxy.stop();
    gpsPort.powerOff();

    isRecording = false;
}

void powerOff()
{
    // TODO: redirect somewhere
    Template::redirect(http.server, "/device/please_wait");

    stopRecording();

    // nmeaServer.stop();        
    statusLed.off();

    // http.stop();     
    // wifiMode.OFF();

    taskStatusLedBlink.disable();
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html
    // rtc_gpio_isolate(GPIO_NUM_12);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    // esp_deep_sleep_start();
}

void restart()
{
    Template::redirect(http.server, "/device/please_wait");

    esp_restart();
}

void buttonPressOneSec()
{
    isRecording ? stopRecording() : startRecording();
}

void buttonPressFiveSec() 
{
    switch (wifiMode.currentMode) {
        case WIFI_AP:
            wifiMode.mode(WIFI_STA);
            break;
        case WIFI_STA:
            wifiMode.mode(WIFI_OFF);
            break;
        case WIFI_OFF:
            wifiMode.mode(WIFI_AP);
            break;
    }
}

void buttonPressTenSec() 
{
    powerOff();
}

void setup()
{
    SerialMonitor.begin(LOG_BAUD_RATE);
    // log_d("ESP32 SDK: %s", ESP.getSdkVersion());
    // log_d("Arduino sketch: %s", __FILE__);
    // log_d("Compiled on: %s", __DATE__);

    // SDCard start & check for updates
    sdCard.start();

    // Load parameters
    params.load();    

    // Initialize sd card
    //sdCard.start();
    // TODO: check for firmware in SDCARD

    // Start wifi & services
    wifiMode.setSTAcredentials(params.storage.wifiStaSsid, params.storage.wifiStaPass);
    wifiMode.mode(params.storage.wifiMode);

    // Start webServer
    // TODO: check if WIFI is up
    // Bind some web functions
    http.on("/device/poweroff", powerOff);
    http.on("/device/restart", restart);
    http.start();

    // NMEAServer
    nmeaServer.start();

    // Button control
    button.begin();
    button.onPressedFor(1000, buttonPressOneSec);
    button.onPressedFor(3000, buttonPressFiveSec);
    button.onPressedFor(10000, buttonPressTenSec);

    // taskStatusLedBlink.setInterval(1000);
    // taskStatusLedBlink.disable();


    statusLed.off();

    // startRecording();

    // Dump information
    // log_d("Battery voltage: %.2f V", battery.voltage());
    // log_d("Battery percentage: %d %%", battery.percentage());

    // log_d("Total heap: %d", ESP.getHeapSize());
    // log_d("Free heap: %d", ESP.getFreeHeap());
    // log_d("Total PSRAM: %d", ESP.getPsramSize());
    // log_d("Free PSRAM: %d", ESP.getFreePsram());
}

void loop()
{
    switch (params.storage.gpsMode) {
        case GPS_MODE_BLE:
            break;
        case GPS_MODE_CSV:
            gpsLogProxy.handleLoop();
            break;
    }   

    http.handleClient();
    nmeaServer.handleClient();

    button.read();

    ts.execute();    
}
