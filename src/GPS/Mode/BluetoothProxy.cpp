#include "GPS/Mode/BluetoothProxy.h"

#define DEVICE_NAME_SIZE 20

BluetoothProxy::BluetoothProxy()
{
    BluetoothSerial serialBT;
}

void BluetoothProxy::start()
{
    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    serialBT.begin(btSsid);

    isProxyStarted = true;
}

void BluetoothProxy::stop()
{
    serialBT.end();
    esp_bt_controller_disable();

    isProxyStarted = false;

    return;
}

void BluetoothProxy::handle(uint8_t *data, size_t size)
{
    if (!isProxyStarted)
    {
        return;
    }

    serialBT.write(data, size);
}
