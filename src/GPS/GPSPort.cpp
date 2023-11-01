#include "GPS/GPSPort.h"

void GPSPort::initialize()
{
    GPSSerial.setRxBufferSize(GPS_RX_BUFFER_SIZE);
    GPSSerial.begin(0, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);
    pushMessage(UBX_CFG_PRT__SETUP, sizeof(UBX_CFG_PRT__SETUP));
    GPSSerial.flush();
    delay(250);
    GPSSerial.end();
    delay(250);
    GPSSerial.begin(0, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);

    start();

    // Setup
    pushMessage(UBX_CFG_GNSS__SETUP, sizeof(UBX_CFG_GNSS__SETUP));
    pushMessage(UBX_CFG_NAV5__SETUP, sizeof(UBX_CFG_NAV5__SETUP));

    // Optimizations
    pushMessage(UBX_CFG_PMS__SETUP, sizeof(UBX_CFG_PMS__SETUP));
    pushMessage(UBX_CFG_PMS__SAVE, sizeof(UBX_CFG_PMS__SAVE));
    pushMessage(UBX_CFG_SBAS__SETUP, sizeof(UBX_CFG_SBAS__SETUP));
    pushMessage(UBX_CFG_NAVX5__SETUP, sizeof(UBX_CFG_NAVX5__SETUP));
    pushMessage(UBX_CFG_ITFM__SETUP, sizeof(UBX_CFG_ITFM__SETUP));

    pushMessage(UBX_CFG_CFG__SAVE, sizeof(UBX_CFG_CFG__SAVE));
}

void GPSPort::pushMessage(const char message[], uint size)
{
    uint8_t CK_A, CK_B;
    size_t ubxMsgSize = size + 4;
    uint8_t ubxMsg[ubxMsgSize];

    // Header
    ubxMsg[0] = 0xB5;
    ubxMsg[1] = 0x62;

    for (uint i = 0; i < size; i++)
    {
        CK_A += message[i];
        CK_B += CK_A;
        ubxMsg[i + 2] = message[i];
    }

    ubxMsg[ubxMsgSize - 2] = CK_A;
    ubxMsg[ubxMsgSize - 1] = CK_B;

    // for (int i = 0; i < ubxMsgSize; i++)
    // {
    //     char buff[8];
    //     sprintf(buff, "%02X ", ubxMsg[i]);
    //     Serial.print(buff);
    // }
    // Serial.println();
    GPSSerial.write(ubxMsg, ubxMsgSize);
}

void GPSPort::setRefreshRate(uint refreshRate)
{
    switch (refreshRate)
    {
    case GPS_RATE_1_HZ:
        pushMessage(UBX_CFG_RATE__1HZ, sizeof(UBX_CFG_RATE__1HZ));
        break;
    case GPS_RATE_5_HZ:
        pushMessage(UBX_CFG_RATE__5HZ, sizeof(UBX_CFG_RATE__5HZ));
        break;
    case GPS_RATE_10_HZ:
        pushMessage(UBX_CFG_RATE__10HZ, sizeof(UBX_CFG_RATE__10HZ));
        break;
    }
}

void GPSPort::stop()
{
    pushMessage(UBX_CFG_RST__GNSS_STOP, sizeof(UBX_CFG_RST__GNSS_STOP));
}

void GPSPort::start()
{
    pushMessage(UBX_CFG_RST__GNSS_START, sizeof(UBX_CFG_RST__GNSS_START));
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
    if (app == GPS_OPTIMIZE_FOR_U_CENTER)
    {
        return;
    }

    // Leave RMC - recommended minimum
    pushMessage(UBX_CFG_MSG__GxRMC_ON, sizeof(UBX_CFG_MSG__GxRMC_ON));

    // Turn off all messages
    // NMEA
    pushMessage(UBX_CFG_MSG__GxGGA_OFF, sizeof(UBX_CFG_MSG__GxGGA_OFF));
    pushMessage(UBX_CFG_MSG__GxGLL_OFF, sizeof(UBX_CFG_MSG__GxGLL_OFF));
    pushMessage(UBX_CFG_MSG__GxGSA_OFF, sizeof(UBX_CFG_MSG__GxGSA_OFF));
    pushMessage(UBX_CFG_MSG__GxGSV_OFF, sizeof(UBX_CFG_MSG__GxGSV_OFF));
    pushMessage(UBX_CFG_MSG__GxVTG_OFF, sizeof(UBX_CFG_MSG__GxVTG_OFF));
    pushMessage(UBX_CFG_MSG__GxGRS_OFF, sizeof(UBX_CFG_MSG__GxGRS_OFF));
    pushMessage(UBX_CFG_MSG__GxGST_OFF, sizeof(UBX_CFG_MSG__GxGST_OFF));
    pushMessage(UBX_CFG_MSG__GxZDA_OFF, sizeof(UBX_CFG_MSG__GxZDA_OFF));
    pushMessage(UBX_CFG_MSG__GxGBS_OFF, sizeof(UBX_CFG_MSG__GxGBS_OFF));
    pushMessage(UBX_CFG_MSG__GxDTM_OFF, sizeof(UBX_CFG_MSG__GxDTM_OFF));
    pushMessage(UBX_CFG_MSG__GxGNS_OFF, sizeof(UBX_CFG_MSG__GxGNS_OFF));
    pushMessage(UBX_CFG_MSG__GxVLW_OFF, sizeof(UBX_CFG_MSG__GxVLW_OFF));

    // PUBX
    pushMessage(UBX_CFG_MSG__PUBX00_ON, sizeof(UBX_CFG_MSG__PUBX00_ON));
    pushMessage(UBX_CFG_MSG__PUBX02_OFF, sizeof(UBX_CFG_MSG__PUBX02_OFF));
    pushMessage(UBX_CFG_MSG__PUBX03_OFF, sizeof(UBX_CFG_MSG__PUBX03_OFF));
    pushMessage(UBX_CFG_MSG__PUBX04_OFF, sizeof(UBX_CFG_MSG__PUBX04_OFF));

    switch (app)
    {
    case GPS_OPTIMIZE_FOR_TRACKADDICT:
        // https://forum.hptuners.com/showthread.php?69123-Track-addict-external-GPS&highlight=gps
        pushMessage(UBX_CFG_NMEA__GP_TALKER, sizeof(UBX_CFG_NMEA__GP_TALKER));
        pushMessage(UBX_CFG_MSG__GxGGA_ON, sizeof(UBX_CFG_MSG__GxGGA_ON));
        pushMessage(UBX_CFG_MSG__GxGLL_ON, sizeof(UBX_CFG_MSG__GxGLL_ON));
        break;
    case GPS_OPTIMIZE_FOR_RACECHRONO_GGA:
        // https://racechrono.com/forum/discussion/comment/11252/#Comment_11252
        // https://racechrono.com/forum/discussion/1421/best-settings-for-qstarz818xt
        pushMessage(UBX_CFG_NMEA__GP_TALKER, sizeof(UBX_CFG_NMEA__GP_TALKER));
        pushMessage(UBX_CFG_MSG__GxGGA_ON, sizeof(UBX_CFG_MSG__GxGGA_ON));
        break;
    case GPS_OPTIMIZE_FOR_RACECHRONO_VTG_ZDA:
        pushMessage(UBX_CFG_NMEA__DEFAULT, sizeof(UBX_CFG_NMEA__DEFAULT));
        pushMessage(UBX_CFG_MSG__GxVTG_ON, sizeof(UBX_CFG_MSG__GxVTG_ON));
        pushMessage(UBX_CFG_MSG__GxZDA_ON, sizeof(UBX_CFG_MSG__GxZDA_ON));
        break;
    case GPS_OPTIMIZE_FOR_RACETIME:
        pushMessage(UBX_CFG_NMEA__GP_TALKER, sizeof(UBX_CFG_NMEA__GP_TALKER));
        pushMessage(UBX_CFG_MSG__GxGBS_ON, sizeof(UBX_CFG_MSG__GxGBS_ON));
        pushMessage(UBX_CFG_MSG__GxGGA_ON, sizeof(UBX_CFG_MSG__GxGGA_ON));
        break;
    default:
        // TODO:
        pushMessage(UBX_CFG_NMEA__DEFAULT, sizeof(UBX_CFG_NMEA__DEFAULT));
        break;
    }

    pushMessage(UBX_CFG_CFG__SAVE, sizeof(UBX_CFG_CFG__SAVE));
}
