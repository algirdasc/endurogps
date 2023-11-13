#include "LED.h"

#ifdef ARDUINO_TTGO_T1
#define LED_ON HIGH
#define LED_OFF LOW
#else
#define LED_ON LOW
#define LED_OFF HIGH
#endif

LED::LED(int GPIO)
{
    LED::GPIO = GPIO;    
    pinMode(GPIO, OUTPUT);
}

void LED::toggle()
{
    isOn = !isOn;
    digitalWrite(GPIO, (isOn ? LED_ON : LED_OFF));
}

void LED::off()
{
    digitalWrite(GPIO, LED_OFF);
}

void LED::on()
{
    digitalWrite(GPIO, LED_ON);
}
