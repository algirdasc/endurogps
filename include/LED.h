#pragma once

#include <Arduino.h>

class LED
{
    private:
        bool isOn;
        int GPIO;        

    public:
        LED(int GPIO);

        void off();
        void on();
        void toggle();
};
