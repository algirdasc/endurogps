#include "GPS/GPSPort.h"

void GPSPort::initialize()
{
    GPSSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);
    GPSSerial.setTimeout(GPS_UART_TIMEOUT);

    pushMessage(UBLOX_WARMSTART_WITH_START, sizeof(UBLOX_WARMSTART_WITH_START));

    GPSSerial.flush();
    delay(250);
    GPSSerial.end();
    delay(250);  

    GPSSerial.setRxBufferSize(GPS_RX_BUFFER_SIZE);
    GPSSerial.begin(0, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX, false, GPS_UART_TIMEOUT);
    log_d("Start UART connection on RX pin %d, TX pin %d and autobaudrate", GPIO_GPS_RX, GPIO_GPS_TX);
    if (GPSSerial.baudRate() > 0) {
        log_d("Connected with auto-baudrate at %u", GPSSerial.baudRate());
    } else {
        log_e("Can't auto find BAUD rate, forcing %u", GPS_BAUD_RATE);
        // TODO: enable pulsing error on the LED to signal the user that something is bad
        GPSSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);
    }
}

void GPSPort::pushMessage(const char message[], uint size)
{
    for (uint i = 0; i < size; i++) {
        GPSSerial.write(message[i]);
    }
}

void GPSPort::setBaudrate(uint32_t baudRate)
{
    switch (baudRate) {
        case 38400:
            pushMessage(UBLOX_BAUD_38400, sizeof(UBLOX_BAUD_38400));
            break;
        case 57600:
            pushMessage(UBLOX_BAUD_57600, sizeof(UBLOX_BAUD_57600));
            break;
        case 115200:
            pushMessage(UBLOX_BAUD_115200, sizeof(UBLOX_BAUD_115200));
            break;
        default:
            pushMessage(UBLOX_BAUD_38400, sizeof(UBLOX_BAUD_38400));
            break;
    }

    GPSSerial.flush();
    delay(500);

    GPSSerial.updateBaudRate(baudRate);
}

void GPSPort::setRate(uint rate)
{
    switch (rate) {
        case 1:
            log_d("Seting 1hz");
            pushMessage(UBLOX_INIT_1HZ, sizeof(UBLOX_INIT_1HZ));
            break;
        case 5:
            log_d("Seting 5hz");
            pushMessage(UBLOX_INIT_5HZ, sizeof(UBLOX_INIT_5HZ));
            break;
        case 10:
            log_d("Seting 10hz");
            pushMessage(UBLOX_INIT_10HZ, sizeof(UBLOX_INIT_10HZ));
            break;
    }
}

void GPSPort::stop()
{
    pushMessage(UBLOX_PWR_STOP, sizeof(UBLOX_PWR_STOP));
}

void GPSPort::start()
{
    pushMessage(UBLOX_PWR_START, sizeof(UBLOX_PWR_START));
}

void GPSPort::setGSV(bool enabled)
{
    // enabled ? pushMessage(UBLOX_GxGSV_ON, sizeof(UBLOX_GxGSV_ON)) : pushMessage(UBLOX_GxGSV_OFF, sizeof(UBLOX_GxGSV_OFF));
}

void GPSPort::setGSA(bool enabled)
{
    // enabled ? pushMessage(UBLOX_GxGSA_ON, sizeof(UBLOX_GxGSA_ON)) : pushMessage(UBLOX_GxGSA_OFF, sizeof(UBLOX_GxGSA_OFF));
}

void GPSPort::setGBS(bool enabled)
{
    // enabled ? pushMessage(UBLOX_GxGBS_ON, sizeof(UBLOX_GxGBS_ON)) : pushMessage(UBLOX_GxGBS_OFF, sizeof(UBLOX_GxGBS_OFF));
}

void GPSPort::setGLL(bool enabled)
{
    // enabled ? pushMessage(UBLOX_GxGLL_ON, sizeof(UBLOX_GxGLL_ON)) : pushMessage(UBLOX_GxGLL_OFF, sizeof(UBLOX_GxGLL_OFF));
}

void GPSPort::setVTG(bool enabled)
{
    // enabled ? pushMessage(UBLOX_GxVTG_ON, sizeof(UBLOX_GxVTG_ON)) : pushMessage(UBLOX_GxVTG_OFF, sizeof(UBLOX_GxVTG_OFF));
}

void GPSPort::setMainTalker(uint talkerID)
{
    // switch (talkerID) {
    //     case GPSPORT_MAINTALKER_GP:
    //         pushMessage(UBLOX_INIT_MAINTALKER_GP, sizeof(UBLOX_INIT_MAINTALKER_GP));
    //         break;
    //     case GPSPORT_MAINTALKER_GP_GPSONLY:
    //         pushMessage(UBLOX_INIT_MAINTALKER_GP_GPSONLY, sizeof(UBLOX_INIT_MAINTALKER_GP_GPSONLY));
    //         break;            
    // }    
}

void GPSPort::setPowerSave(uint timeS)
{
    // switch (timeS) {
    //     case 1800:
    //         pushMessage(UBLOX_PWR_SAVE_30MIN, sizeof(UBLOX_PWR_SAVE_30MIN));
    //         break;
    //     case 3600:
    //         pushMessage(UBLOX_PWR_SAVE_1HR, sizeof(UBLOX_PWR_SAVE_1HR));
    //         break;
    // }
}

void GPSPort::setSVChannels(uint channels)
{
    // switch (channels) {
    //     case 8:
    //         pushMessage(UBLOX_INIT_CHANNEL_8, sizeof(UBLOX_INIT_CHANNEL_8));
    //         break;
    //     case 12:
    //         pushMessage(UBLOX_INIT_CHANNEL_12, sizeof(UBLOX_INIT_CHANNEL_12));
    //         break;
    //     case 16:
    //         pushMessage(UBLOX_INIT_CHANNEL_16, sizeof(UBLOX_INIT_CHANNEL_16));
    //         break;
    //     default:
    //         pushMessage(UBLOX_INIT_CHANNEL_ALL, sizeof(UBLOX_INIT_CHANNEL_ALL));
    //         break;
    // }
}
