#include "LED.h"

LED::LED(int GPIO)
{
    // Task LedBlinkTask(0, TASK_FOREVER, &LED::blink, &ts, false);

    LED::GPIO = GPIO;    
    pinMode(GPIO, OUTPUT);
    // LED::LedBlinkTask = LedBlinkTask;
}

void LED::toggle()
{
    isOn = !isOn;
    digitalWrite(LED::GPIO, (isOn ? HIGH : LOW));
}

void LED::blink()
{
    // LedBlinkTask.setInterval(interval);
    // LedBlinkTask.enable();
}

void LED::off()
{
    // LedBlinkTask.disable();
    digitalWrite(LED::GPIO, HIGH);
}
