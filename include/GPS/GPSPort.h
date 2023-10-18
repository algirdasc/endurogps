#include <Arduino.h>
#include "GPS/UBLOXMessages.h"

// TODO: move to params?
#define GPSPORT_MAINTALKER_GP 0
#define GPSPORT_MAINTALKER_GP_GPSONLY 1
#define GPSPORT_POWER_SAVE 1800
#define GPSPORT_SV_CHANNELS 0

class GPSPort
{    
    public:
        void initialize();
        void pushMessage(const char message[]);
        void powerOff();
        void setBaudrate(uint32_t baudRate);
        void setRate(uint rate);        
        void setGSV(bool enabled);
        void setGSA(bool enabled);
        void setGBS(bool enabled);
        void setGLL(bool enabled);
        void setVTG(bool enabled);
        void setMainTalker(uint talkerID);
        void setPowerSave(uint timeS);
        void setSVChannels(uint channels);
};
