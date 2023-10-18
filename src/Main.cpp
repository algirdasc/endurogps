#include "Main.h"

Scheduler ts;
WifiMode wifiMode;
EasyButton button(GPIO_BUTTON);
LED statusLed(LED_BUILTIN);
SDCard sdCard;
NMEAServer nmeaServer(NMEA_SERVER_PORT);
GPSPort gpsPort;
Params params;
Battery battery;
HTTP http(ENDUROGPS_HTTP_PORT);
GPSBLEProxy gpsBLEProxy;
GPSLogProxy gpsLogProxy;

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
}

void stopRecording()
{
    // gpsBLEProxy.stop();
    // gpsLogProxy.stop();
    gpsPort.powerOff();
}

void powerOff()
{
    // TODO: redirect somewhere
    Template::redirect(http.server, "/device/powered_off");

    stopRecording();

    nmeaServer.stop();        
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

void setup()
{
    SerialMonitor.begin(LOG_BAUD_RATE);
    delay(200);    
    log_d("ESP32 SDK: %s", ESP.getSdkVersion());
    log_d("Arduino sketch: %s", __FILE__);
    log_d("Compiled on: %s", __DATE__);

    // Bind some web functions
    http.on("/device/poweroff", powerOff);
    http.on("/device/restart", esp_restart);

    // Load parameters
    params.load();    

    sdCard.start();

    // Start wifi & services
    wifiMode.start(params.storage.wifiMode);
    http.start();

    MDNS.begin(ENDUROGPS_AP_MDNS);
    MDNS.addService("http", "tcp", ENDUROGPS_HTTP_PORT);

    button.begin();
    //button.onPressedFor(1000, onPressedForDuration);
    //button.onPressedFor(5000, onPressedForDuration);
    //button.onPressedFor(10000, onPressedForDuration);

    nmeaServer.start();

    taskStatusLedBlink.setInterval(1000);
    taskStatusLedBlink.disable();

    statusLed.off();

    // startRecording();

    // Dump information
    log_d("Battery voltage: %.2f V", battery.voltage());
    log_d("Battery percentage: %d %%", battery.percentage());

    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
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
