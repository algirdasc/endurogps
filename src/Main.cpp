#include <Arduino.h>
#include <ESPmDNS.h>

#include "EnduroGPS.h"
#include "Main.h"

Scheduler ts;
Settings settings;
HTTP http(ENDUROGPS_HTTP_PORT);
WifiMode wifiMode;
EasyButton button(GPIO_BUTTON);
Battery battery;
LED statusLed(LED_BUILTIN);
SDCard sdCard;


void ledBlink()
{
    statusLed.toggle();
}
Task taskStatusLedBlink(0, TASK_FOREVER, &ledBlink, &ts, false);


void setup()
{
    SerialMonitor.begin(LOG_BAUD_RATE);
    delay(200);
    log_d("ESP32 SDK: %s", ESP.getSdkVersion());
    log_d("Arduino sketch: %s", __FILE__);
    log_d("Compiled on: %s", __DATE__);

    wifiMode.start(settings.get().wifiMode);

    http.start();

    MDNS.addService("http", "tcp", ENDUROGPS_HTTP_PORT);

    button.begin();
    //button.onPressedFor(1000, onPressedForDuration);
    //button.onPressedFor(5000, onPressedForDuration);
    //button.onPressedFor(10000, onPressedForDuration);

    log_d("Battery voltage: %.2f V", battery.voltage());
    log_d("Battery percentage: %d %%", battery.percentage());

    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());

    taskStatusLedBlink.setInterval(500);
    taskStatusLedBlink.enable();
}

void loop()
{
    // if (wifiMode.isConnected) {
    http.handleClient();
    // }

    // Read button
    button.read();

    // Execute periodic tasks
    ts.execute();
}
