#include "GPS/GPSPort.h"
#include "EnduroGPS.h"

void GPSPort::initialize()
{    
    GPSSerial.begin(GPS_STANDARD_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);

    pushMessage(UBLOX_WARMSTART);

    GPSSerial.flush();
    delay(250);
    GPSSerial.end();
    delay(250);

    GPSSerial.setRxBufferSize(UART_BUFFER_SIZE_RX);

    GPSSerial.begin(0, SERIAL_8N1, GPS_RX, GPS_TX, false, GPS_UART_TIMEOUT);
    log_d("Start UART connection on RX pin %d, TX pin %d and autobaudrate", GPS_RX, GPS_TX);
    if (GPSSerial.baudRate() > 0) {
        log_d("Connected with auto-baudrate at %u", GPSSerial.baudRate());
    } else {
        log_e("Can't auto find BAUD rate, forcing %u", GPS_STANDARD_BAUD_RATE);
        // TODO: enable pulsing error on the LED to signal the user that something is bad
        GPSSerial.begin(GPS_STANDARD_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX);
    }
}

void GPSPort::pushMessage(const char message[])
{
    // if (gps_powersave)
    // {
    //     log_d("Disable powermode");
    //     gpsPort.end();
    //     delay(250);
    //     // assumes stored_preferences global
    //     gps_powersave = false;
    //     gps_initialize_settings();
    // }

    uint messageSize = sizeof(message);

    for (uint i = 0; i < messageSize; i++) {
        GPSSerial.write(message[i]);
    }
}

void GPSPort::setBaudrate(uint32_t baudRate)
{
    switch (baudRate) {
        case 38400:
            pushMessage(UBLOX_BAUD_38400);
            break;
        case 57600:
            pushMessage(UBLOX_BAUD_57600);
            break;
        case 115200:
            pushMessage(UBLOX_BAUD_115200);
            break;
        default:
            pushMessage(UBLOX_BAUD_38400);
            break;
    }

    GPSSerial.flush();
    delay(500);
    log_d("Changing baud on UART2 from %u to %u", GPSSerial.baudRate(), baudRate);

    GPSSerial.updateBaudRate(baudRate);
}

void GPSPort::setRate(uint rate)
{
    switch (rate) {
        case 1:
            pushMessage(UBLOX_INIT_1HZ);
            break;
        case 5:
            pushMessage(UBLOX_INIT_5HZ);
            break;
        case 10:
            pushMessage(UBLOX_INIT_10HZ);
            break;
        default:
            log_e("Invalid GPS rate %d", rate);
            break;
    }

    log_d("GPS rate set %dHz", rate);
}

void GPSPort::powerOff()
{
    pushMessage(UBLOX_PWR_OFF);
}

void GPSPort::setGSV(bool enabled)
{
    log_d("GSV switched to %d", enabled);
    enabled ? pushMessage(UBLOX_GxGSV_ON) : pushMessage(UBLOX_GxGSV_OFF);
}

void GPSPort::setGSA(bool enabled)
{
    log_d("GSA switched to %d", enabled);
    enabled ? pushMessage(UBLOX_GxGSA_ON) : pushMessage(UBLOX_GxGSA_OFF);
}

void GPSPort::setGBS(bool enabled)
{
    log_d("GBS switched to %d", enabled);
    enabled ? pushMessage(UBLOX_GxGBS_ON) : pushMessage(UBLOX_GxGBS_OFF);
}

void GPSPort::setGLL(bool enabled)
{
    log_d("GLL switched to %d", enabled);
    enabled ? pushMessage(UBLOX_GxGLL_ON) : pushMessage(UBLOX_GxGLL_OFF);
}

void GPSPort::setVTG(bool enabled)
{
    log_d("VTG switched to %d", enabled);
    enabled ? pushMessage(UBLOX_GxVTG_ON) : pushMessage(UBLOX_GxVTG_OFF);
}

void GPSPort::setMainTalker(uint talkerID)
{
    switch (talkerID) {
        case GPSPORT_MAINTALKER_GP:
            log_d("TalkedID switched to GP");
            pushMessage(UBLOX_INIT_MAINTALKER_GP);
            break;
        case GPSPORT_MAINTALKER_GP_GPSONLY:
            log_d("TalkedID switched to GPSONLY");
            pushMessage(UBLOX_INIT_MAINTALKER_GP_GPSONLY);
            break;            
        default:
            log_e("Invalid talkerID %d provided", talkerID);
            break;
    }    
}

void GPSPort::setPowerSave(uint timeS)
{
    switch (timeS) {
        case 1800:
            log_d("PowerSave switched to 1800");
            pushMessage(UBLOX_PWR_SAVE_30MIN);
            break;
        case 3600:
            log_d("PowerSave switched to 3600");
            pushMessage(UBLOX_PWR_SAVE_1HR);
            break;
        default:
            log_e("Invalid power save time %d provided", timeS);
            break;
    }
}

void GPSPort::setSVChannels(uint channels)
{
    switch (channels) {
        case 8:
            pushMessage(UBLOX_INIT_CHANNEL_8);
            break;
        default:
            pushMessage(UBLOX_INIT_CHANNEL_ALL);
            break;
    }

    log_d("SV channels switched to %d", channels);
}
