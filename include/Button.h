#pragma once

#include <Arduino.h>

#define MAX_BUTTON_CALLBACKS 5

typedef void (*callback_t)();

class Button
{
private:
    uint pin;
    uint callbackCount;
    uint callbackDuration[MAX_BUTTON_CALLBACKS];
    uint maxDuration = 0;
    callback_t maxDurationCallbackFn;    
    callback_t callbackFn[MAX_BUTTON_CALLBACKS];
    uint32_t pressedAt;
    bool stopReading = false;

    int readPin();
    void handle(uint32_t pressDuration, bool isPressed);
    int lastState;

public:   
    Button(uint pin);

    void begin();
    void read();
    void onPressedFor(uint duration, callback_t callback);
};
