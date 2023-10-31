#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Update.h>

#include "EnduroGPS.h"

class SDCard
{
    private:
        void fwUpdateCheck();
        void fwUpdate(Stream &updateFile, size_t updateSize);
    public:
        void start();
};
