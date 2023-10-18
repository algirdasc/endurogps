#include <Arduino.h>
#include <EasyButton.h>
#include <TaskScheduler.h>
#include <ESPmDNS.h>

#include "EnduroGPS.h"
#include "HTTP/Template.h"
#include "Params.h"
#include "HTTP.h"
#include "Battery.h"
#include "WifiMode.h"
#include "LED.h"
#include "SDCard.h"
#include "NMEAServer.h"
#include "GPS/GPSPort.h"
#include "GPS/Mode/GPSLogProxy.h"
#include "GPS/Mode/GPSBLEProxy.h"