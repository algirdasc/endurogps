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
    switch (refreshRate) {
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
