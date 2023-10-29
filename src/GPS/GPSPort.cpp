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
    if (GPSSerial.baudRate() > 0)
    {
        log_d("Connected with auto-baudrate at %u", GPSSerial.baudRate());
    }
    else
    {
        log_e("Can't auto find BAUD rate, forcing %u", GPS_BAUD_RATE);
        // TODO: enable pulsing error on the LED to signal the user that something is bad
        GPSSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);
    }
}

void GPSPort::pushMessage(const char message[], uint size)
{
    for (uint i = 0; i < size; i++)
    {
        GPSSerial.write(message[i]);
    }
}

void GPSPort::setBaudrate(uint32_t baudRate)
{
    switch (baudRate)
    {
    case GPS_RATE_38400:
        pushMessage(UBLOX_BAUD_38400, sizeof(UBLOX_BAUD_38400));
        break;
    case GPS_RATE_57600:
        pushMessage(UBLOX_BAUD_57600, sizeof(UBLOX_BAUD_57600));
        break;
    case GPS_RATE_115200:
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

void GPSPort::setRefreshRate(uint refreshRate)
{
    switch (refreshRate)
    {
    case GPS_RATE_1_HZ:
        pushMessage(UBLOX_INIT_1HZ, sizeof(UBLOX_INIT_1HZ));
        break;
    case GPS_RATE_5_HZ:
        pushMessage(UBLOX_INIT_5HZ, sizeof(UBLOX_INIT_5HZ));
        break;
    case GPS_RATE_10_HZ:
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

void GPSPort::optimizeFor(uint app)
{
    switch (app)
    {
        case GPS_OPTIMIZE_FOR_TRACKADDICT:
            // https://forum.hptuners.com/showthread.php?69123-Track-addict-external-GPS&highlight=gps
            pushMessage(UBLOX_TRACKADDICT_CHANNELS, sizeof(UBLOX_TRACKADDICT_CHANNELS));
            pushMessage(UBLOX_GxGLL_ON, sizeof(UBLOX_GxGLL_ON));
            pushMessage(UBLOX_GxVTG_OFF, sizeof(UBLOX_GxVTG_OFF));
            pushMessage(UBLOX_GxGSA_OFF, sizeof(UBLOX_GxGSA_OFF));
            pushMessage(UBLOX_GxGSV_OFF, sizeof(UBLOX_GxGSV_OFF));
            pushMessage(UBLOX_GxGBS_OFF, sizeof(UBLOX_GxGBS_OFF));
            break;
        case GPS_OPTIMIZE_FOR_RACECHRONO:
            // https://racechrono.com/forum/discussion/comment/11252/#Comment_11252
            // https://racechrono.com/forum/discussion/1421/best-settings-for-qstarz818xt
            pushMessage(UBLOX_RACECHRONO_CHANNELS, sizeof(UBLOX_RACECHRONO_CHANNELS));
            pushMessage(UBLOX_GxGLL_OFF, sizeof(UBLOX_GxGLL_OFF));
            pushMessage(UBLOX_GxVTG_OFF, sizeof(UBLOX_GxVTG_OFF));
            pushMessage(UBLOX_GxGSA_ON, sizeof(UBLOX_GxGSA_ON));
            pushMessage(UBLOX_GxGSV_ON, sizeof(UBLOX_GxGSV_ON));
            pushMessage(UBLOX_GxGBS_OFF, sizeof(UBLOX_GxGBS_OFF));
            break;
        case GPS_OPTIMIZE_FOR_RACETIME:
            pushMessage(UBLOX_GxGLL_ON, sizeof(UBLOX_GxGLL_ON));
            pushMessage(UBLOX_GxVTG_ON, sizeof(UBLOX_GxVTG_ON));
            pushMessage(UBLOX_GxGSA_OFF, sizeof(UBLOX_GxGSA_OFF));
            pushMessage(UBLOX_GxGSV_OFF, sizeof(UBLOX_GxGSV_OFF));
            pushMessage(UBLOX_GxGBS_OFF, sizeof(UBLOX_GxGBS_OFF));
            break;
        default:
            break;
    }
}
