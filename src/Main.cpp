#include "Main.h"
#include "Globals.h"

// Initialize globals
bool g_isRecording = false;
bool g_isLocationValid = false;

Scheduler ts;
WifiMode wifiMode;
EasyButton button(GPIO_BUTTON);
LED statusLed(GPIO_LED);
SDCard sdCard;
AsyncServer *server = new AsyncServer(NMEA_PORT);
GPSPort gpsPort;
Params params;
Battery battery;
WebServer webserver;
BluetoothProxy gpsBTProxy;
NeoGPSProxy gpsNeoGPSProxy;

static std::vector<AsyncClient *> NMEAClients;

void ledToggle()
{
    statusLed.toggle();
}
Task taskStatusLedBlink(0, TASK_FOREVER, &ledToggle, &ts, false);
Task taskReboot(0, TASK_ONCE, &esp_restart, &ts, false);

void setLedStatus()
{
    if (g_isRecording && !g_isLocationValid) {
        taskStatusLedBlink.setInterval(200);
        taskStatusLedBlink.enable();
    } else if (g_isRecording && g_isLocationValid) {
        taskStatusLedBlink.setInterval(2000);
        taskStatusLedBlink.enable();
    } else {
        taskStatusLedBlink.disable();
    }
}
Task taskSetLedStatus(1000, TASK_FOREVER, &setLedStatus, &ts, true);

void gpsInitialize()
{
    gpsPort.initialize();
    gpsPort.setRefreshRate(params.storage.gpsRateHz);
    gpsPort.optimizeFor(params.storage.gpsOptimizeFor);
}

void startRecording()
{
    params.load();   
    if (webserver.arg("sessionName").length())
    {
        params.storage.gpsSessionName = HTTP::escape(webserver.arg("sessionName").c_str());
        params.save();
    }
    
    gpsPort.start();

    switch (params.storage.gpsMode)
    {
    case GPS_MODE_BT:
        gpsBTProxy.start();
        break;
    case GPS_MODE_SDCARD:
        gpsNeoGPSProxy.formatter(params.storage.logFormat, params.storage.gpsSessionName.c_str());
        gpsNeoGPSProxy.start();
        break;
    }

    HTTP::redirect(webserver);
}

void stopRecording()
{
    gpsBTProxy.stop();
    gpsNeoGPSProxy.stop();

    HTTP::redirect(webserver);
}

void tPowerOff()
{
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
Task taskPowerOff(0, TASK_ONCE, &tPowerOff, &ts, false);

static void NMEAClientReply(uint8_t *data, size_t size)
{
    for (uint i = 0; i < NMEAClients.size(); i++)
    {
        if (NMEAClients[i]->space() > size && NMEAClients[i]->canSend())
        {
            NMEAClients[i]->write((char *)data, size);
        }
    }
}

static void NMEAHandleClientData(void *arg, AsyncClient *client, void *data, size_t len)
{
    GPSSerial.write((uint8_t *)data, len);
}

static void NMEAHandleClientDisconnect(void *arg, AsyncClient *client)
{
    for (uint i = 0; i < NMEAClients.size(); i++)
    {
        if (NMEAClients[i]->disconnected())
        {
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

void HTTPNotFound()
{
    webserver.send(HTTP_CODE_NOT_FOUND, "text/html", "<h1> 404 Not found</h1><p>The requested resource was not found on this server.</p>");
}

void HTTPPleaseWait()
{
    HTTP::beginPage(webserver);
    webserver.sendContent(R"raw(<div class="page-header"><h1>Please wait</h1></div><div class="alert alert-info">Device is busy... Redirect after <span id="rt">15</span> s.</div>)raw");
    webserver.sendContent(R"raw(<script>var time = 15;var timer = setInterval(function(){time--;document.getElementById("rt").innerHTML=time;if(time == 0){window.location.pathname='/';clearInterval(timer);}},1000);</script>)raw");
    HTTP::endPage(webserver);
}

void HTTPRestart()
{
    HTTPPleaseWait();

    taskReboot.enableDelayed(2000);
}

void HTTPPowerOff()
{
    HTTPPleaseWait();

    taskPowerOff.enableDelayed(2000);
}

void HTTPServerStart()
{
    webserver.onNotFound(HTTPNotFound);

    webserver.on("/device/poweroff", HTTPPowerOff);
    webserver.on("/device/restart", HTTPRestart);

    webserver.on("/device/recording/start", startRecording);
    webserver.on("/device/recording/stop", stopRecording);

    webserver.addHandler(new AssetHandler());
    webserver.addHandler(new IndexHandler());
    webserver.addHandler(new SettingsGPSHandler());
    webserver.addHandler(new SettingsWifiHandler());
    webserver.addHandler(new SettingsTrackHandler());
    webserver.addHandler(new FileBrowserHandler());

    const char *headers[] = {"Referer"};
    webserver.collectHeaders(headers, 1);

    webserver.begin(HTTP_PORT);
}

void HTTPServerStop()
{
    webserver.stop();
}

void handleGPSPort()
{
    size_t len = GPSSerial.available();
    if (!len)
    {
        return;
    }

    uint8_t buf[len];
    GPSSerial.readBytes(buf, len);

    if (server->status() == 1)
    {
        NMEAClientReply(buf, len);
    }

    switch (params.storage.gpsMode)
    {
    case GPS_MODE_BT:
        gpsBTProxy.handleProxy(buf, len);
        break;
    case GPS_MODE_SDCARD:
        gpsNeoGPSProxy.handleProxy(buf, len);
        break;
    }


}

void wifiSetMode(WiFiMode_t mode)
{
    wifiMode.setMode(mode);

    if (mode == WIFI_OFF)
    {
        NMEAServerStop();
        HTTPServerStop();
    }
    else
    {
        if (params.storage.nmeaTcpEnabled)
        {
            NMEAServerStart();
        }

        HTTPServerStart();
    }
}

void buttonPressOneSec()
{
    g_isRecording ? stopRecording() : startRecording();
}

void buttonPressTenSec()
{
    HTTPPowerOff();
}

void buttonPressFiveSec()
{
    switch (wifiMode.currentMode)
    {
    case WIFI_AP:
        wifiSetMode(WIFI_STA);
        break;
    case WIFI_STA:
        wifiSetMode(WIFI_OFF);
        break;
    case WIFI_OFF:
        wifiSetMode(WIFI_AP);
        break;
    case WIFI_MODE_APSTA:
    case WIFI_MODE_MAX:
        break;
    }
}

void setup()
{
    SerialMonitor.begin(LOG_BAUD_RATE);

    // SDCard start & check for updates
    sdCard.start();

    // Load parameters
    params.load();

    // Initialize GPS
    gpsInitialize();

    // Set wifi & bt credentials
    wifiMode.staSsid = params.storage.wifiStaSsid;
    wifiMode.staPass = params.storage.wifiStaPass;
    wifiMode.apSsid = params.storage.wifiApSsid;
    wifiMode.apPass = params.storage.wifiApPass;
    wifiMode.fallbackToAP = params.storage.wifiFallbackAp;
    gpsBTProxy.btSsid = params.storage.wifiApSsid;

    // Start wifi & services
    wifiSetMode(params.storage.wifiMode);

    // Button control
    button.begin();
    button.onPressedFor(1000, buttonPressOneSec);
    button.onPressedFor(3000, buttonPressFiveSec);
    button.onPressedFor(10000, buttonPressTenSec);

    statusLed.off();
}

void loop()
{
    handleGPSPort();

    webserver.handleClient();

    button.read();

    ts.execute();
}
