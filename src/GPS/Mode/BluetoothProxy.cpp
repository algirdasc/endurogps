#include "GPS/Mode/BluetoothProxy.h"

BluetoothProxy::BluetoothProxy()
{
    BluetoothSerial serialBT;
}

void BluetoothProxy::start()
{
    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
    serialBT.begin(btSsid);

    isProxyStarted = true;
    g_isRecording = true;
}

void BluetoothProxy::stop()
{
    serialBT.end();
    esp_bt_controller_disable();

    isProxyStarted = false;
    g_isRecording = false;
}

void BluetoothProxy::handleProxy(uint8_t *data, size_t size)
{
    if (!isProxyStarted)
    {
        return;
    }

    serialBT.write(data, size);
}
