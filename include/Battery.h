#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

class Battery
{
    public:
        float voltage();
        uint8_t percentage();
};

#endif
