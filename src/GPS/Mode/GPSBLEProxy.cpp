#include "GPS/Mode/GPSBLEProxy.h"

GPSBLEProxy::GPSBLEProxy()
{
    BluetoothSerial serialBT;    
}

void GPSBLEProxy::start()
{
    serialBT.begin();
    // serialBT.register_callback(std::bind(&BLEProxy::callback, this));
}

bool GPSBLEProxy::stop()
{
    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE) {        
        log_i("BT is stopped");
        return true;
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) {    
        log_i("BT is enabled");
        
        if (esp_bt_controller_disable()) {        
            log_i("BT disable failed");
            return false;
        }

        while (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED)
        ;
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED) {
        log_i("BT is inited");
        if (esp_bt_controller_deinit()) {
            log_e("BT deint failed");
            return false;
        }

        while (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED)
        ;

        return true;
    }

    return false;
}

void GPSBLEProxy::callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event) {
        case ESP_SPP_SRV_OPEN_EVT:
            deviceConnected = true;
            break;
        case ESP_SPP_CLOSE_EVT:
            deviceConnected = false;
            break;
    }
}
