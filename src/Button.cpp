#include "Button.h"

Button::Button(uint pin)
{
    Button::pin = pin;
}

void Button::begin()
{
    pinMode(pin, INPUT_PULLUP);
    lastState = HIGH;
}

int Button::readPin()
{
    return digitalRead(pin);
}

void Button::read()
{
    int buttonState = readPin();

    if (buttonState != lastState)
    {
        if (buttonState == HIGH)
        {
            if (stopReading)
            {
                stopReading = false;
            }
            else
            {
                handle(millis() - pressedAt, false);
            }
        }
        else
        {
            pressedAt = millis();
        }
    }

    if (buttonState == LOW && !stopReading)
    {
        handle(millis() - pressedAt, true);
    }

    lastState = buttonState;
}

void Button::handle(uint32_t pressDuration, bool isPressed)
{
    uint closest = 60000;
    callback_t closestCallback;

    if (isPressed)
    {
        if (pressDuration >= maxDuration)
        {
            maxDurationCallbackFn();
            stopReading = true;
            return;
        }
    }
    else
    {
        for (uint c = 0; c < callbackCount; c++)
        {
            int32_t difference = callbackDuration[c] - pressDuration;
            difference = abs(difference);
            if (difference < closest)
            {                
                closestCallback = callbackFn[c];
                closest = difference;
            }
        }

        closestCallback();
    }
}

void Button::onPressedFor(uint duration, callback_t callback)
{
    callbackDuration[callbackCount] = duration;
    callbackFn[callbackCount] = callback;
    callbackCount++;

    if (duration > maxDuration)
    {
        maxDuration = duration;
        maxDurationCallbackFn = callback;
    }
}