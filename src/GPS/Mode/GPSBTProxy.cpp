#include "GPS/Mode/GPSBTProxy.h"

GPSBTProxy::GPSBTProxy()
{
    BluetoothSerial serialBT;    
}

void GPSBTProxy::start()
{
    uint16_t chip = (uint16_t)((uint64_t) ESP.getEfuseMac() >> 32);
    sprintf(deviceID, "%s-%04X", DEVICE_NAME, chip);

    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    serialBT.begin(deviceID);

    isStarted = true;
}

void GPSBTProxy::stop()
{
    serialBT.end();
    esp_bt_controller_disable();

    isStarted = false;
    return;
}

void GPSBTProxy::handle(uint8_t *data, size_t size)
{
    if (!isStarted) {
        return;
    }

    serialBT.write(data, size);
}
