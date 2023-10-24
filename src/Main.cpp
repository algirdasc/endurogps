#include "Main.h"

bool isRecording = false;

Scheduler ts;
WifiMode wifiMode;
EasyButton button(GPIO_BUTTON);
LED statusLed(LED_BUILTIN);
SDCard sdCard;
AsyncServer *server = new AsyncServer(NMEA_PORT);
GPSPort gpsPort;
Params params;
Battery battery;
HTTP http;
GPSBTProxy gpsBTProxy;
GPSLogProxy gpsLogProxy;

static std::vector<AsyncClient*> NMEAClients;

void ledBlink()
{
    statusLed.toggle();
}
Task taskStatusLedBlink(0, TASK_FOREVER, &ledBlink, &ts, false);

void gpsInitialize()
{    
    gpsPort.initialize();
    gpsPort.setBaudrate(params.storage.gpsBaudRate);
    gpsPort.setRate(params.storage.gpsRateHz);
}

void startRecording()
{
    gpsPort.start();

    switch (params.storage.gpsMode) {
        case GPS_MODE_BT:
            gpsBTProxy.start();
            break;
        case GPS_MODE_CSV:
            gpsLogProxy.formatter(params.storage.logFormat);
            gpsLogProxy.start();
            break;
    }
    
    taskStatusLedBlink.enable();

    isRecording = true;
}

void stopRecording()
{
    gpsBTProxy.stop();
    gpsLogProxy.stop();    
       
    taskStatusLedBlink.disable();
    statusLed.off();

    isRecording = false;
}

static void NMEAClientReply(uint8_t *data, size_t size)
{    
    for (uint i = 0; i < NMEAClients.size(); i++) {
        if (NMEAClients[i]->space() > size && NMEAClients[i]->canSend()) {
            NMEAClients[i]->write((char *) data, size);
        }
    }
}

static void NMEAHandleClientData(void *arg, AsyncClient *client, void *data, size_t len) 
{
    GPSSerial.write((uint8_t*) data, len);
}

static void NMEAHandleClientDisconnect(void *arg, AsyncClient *client)
{
    for (uint i = 0; i < NMEAClients.size(); i++) {
        if (NMEAClients[i]->disconnected()) {
            NMEAClients.erase(NMEAClients.begin() + i);
        }
    }
}

static void NMEAHandleClient(void *arg, AsyncClient *client)
{
    NMEAClients.push_back(client);

    client->onData(&NMEAHandleClientData, NULL);
    client->onDisconnect(&NMEAHandleClientDisconnect, NULL);
}

void NMEAServerStart()
{
    server->onClient(&NMEAHandleClient, server);
    server->begin();
    server->setNoDelay(true);
}

void NMEAServerStop() 
{
    server->end();
}

void handleGPSPort()
{
    size_t len = GPSSerial.available();
    if (!len) {
        return;
    }

    uint8_t buf[len];
    GPSSerial.readBytes(buf, len);

    if (server->status() == 1) {
        NMEAClientReply(buf, len);
    }

    switch (params.storage.gpsMode) {
        case GPS_MODE_BT:
            gpsBTProxy.handle(buf, len);
            break;
        case GPS_MODE_CSV:
            gpsLogProxy.handle(buf, len);
            break;
    }  
}

void wifiSetMode(WiFiMode_t mode)
{
    wifiMode.setMode(mode);

    if (mode == WIFI_OFF) {
        NMEAServerStop();
        http.stop();
    } else {
        if (params.storage.nmeaTcpEnabled) {
            NMEAServerStart();
        }

        http.start();
    }    
}

void restart()
{
    Template::redirect(http.server, PLEASE_WAIT_PAGE_BASE_URL);

    esp_restart();
}

void powerOff()
{
    Template::redirect(http.server, PLEASE_WAIT_PAGE_BASE_URL);

    stopRecording();
    // wifiSetMode(WIFI_OFF);

    gpsPort.stop();

    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html
    // rtc_gpio_isolate(GPIO_NUM_12);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_deep_sleep_start();
}

void buttonPressOneSec()
{
    isRecording ? stopRecording() : startRecording();
}

void buttonPressTenSec() 
{
    powerOff();
}

void buttonPressFiveSec() 
{
    switch (wifiMode.currentMode) {
        case WIFI_AP:
            wifiSetMode(WIFI_STA);
            break;
        case WIFI_STA:
            wifiSetMode(WIFI_OFF);
            break;
        case WIFI_OFF:
            wifiSetMode(WIFI_AP);
            break;
    }
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

    // Initialize GPS
    gpsInitialize();

    // Set wifi credentials
    wifiMode.staSsid = params.storage.wifiStaSsid;
    wifiMode.staPass = params.storage.wifiStaPass;
    wifiMode.fallbackToAP = params.storage.wifiFallbackAp;

    // Start wifi & services
    wifiSetMode(params.storage.wifiMode);

    // Bind some web functions
    http.on(F("/device/poweroff"), powerOff);
    http.on(F("/device/restart"), restart);

    // TODO: remove?
    http.on(F("/gps/start"), startRecording);
    http.on(F("/gps/stop"), stopRecording);

    // Button control
    button.begin();
    button.onPressedFor(1000, buttonPressOneSec);
    button.onPressedFor(3000, buttonPressFiveSec);
    button.onPressedFor(10000, buttonPressTenSec);

    taskStatusLedBlink.setInterval(1000);    
    statusLed.off();

    // Dump information
    // log_d("Battery voltage: %.2f V", battery.voltage());
    // log_d("Battery percentage: %d %%", battery.percentage());
}

void loop()
{    
    handleGPSPort();

    http.handleClient();

    button.read();

    ts.execute();
}
