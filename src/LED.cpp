#include "LED.h"

LED::LED(int GPIO)
{
    LED::GPIO = GPIO;    
    pinMode(GPIO, OUTPUT);
}

void LED::toggle()
{
    isOn = !isOn;
    digitalWrite(LED::GPIO, (isOn ? HIGH : LOW));
}

void LED::off()
{
    // LedBlinkTask.disable();
    digitalWrite(LED::GPIO, HIGH);
}

void LED::on()
{
    // LedBlinkTask.disable();
    digitalWrite(LED::GPIO, LOW);
}
