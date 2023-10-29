#include <Arduino.h>
#include <EasyButton.h>
#include <TaskScheduler.h>
#include <AsyncTCP.h>

#include "EnduroGPS.h"
#include "Params.h"
#include "HTTP.h"
#include "Battery.h"
#include "WifiMode.h"
#include "LED.h"
#include "SDCard.h"
#include "GPS/GPSPort.h"
#include "GPS/Mode/SDCardProxy.h"
#include "GPS/Mode/BluetoothProxy.h"

#include "HTTP/Handlers/IndexHandler.h"
#include "HTTP/Handlers/SettingsWifiHandler.h"
#include "HTTP/Handlers/SettingsGPSHandler.h"
#include "HTTP/Handlers/FileBrowserHandler.h"
#include "HTTP/Handlers/AssetHandler.h"