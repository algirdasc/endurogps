#include <Arduino.h>

class LED
{
    private:
        bool isOn;
        int GPIO;        

    public:
        LED(int GPIO);

        void off();
        void blink();
        void toggle();
};
