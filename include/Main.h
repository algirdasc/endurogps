#include <Arduino.h>
#include <TaskScheduler.h>
#include <AsyncTCP.h>

#include "EnduroGPS.h"
#include "Params.h"
#include "Button.h"
#include "HTTP.h"
#include "Battery.h"
#include "WifiMode.h"
#include "LED.h"
#include "SDCardUpdate.h"
#include "GPS/GPSPort.h"
#include "GPS/Mode/NeoGPSProxy.h"
#include "GPS/Mode/BluetoothProxy.h"

#include "HTTP/Handlers/IndexHandler.h"
#include "HTTP/Handlers/SettingsWifiHandler.h"
#include "HTTP/Handlers/SettingsGPSHandler.h"
#include "HTTP/Handlers/SettingsTrackHandler.h"
#include "HTTP/Handlers/FileBrowserHandler.h"
#include "HTTP/Handlers/AssetHandler.h"

enum Status {
    IDLE,
    RECORDING,
    SIGNAL_LOST,        
};