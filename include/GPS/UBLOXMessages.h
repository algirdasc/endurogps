/********************************

   GPS Settings
   These sentences are used to configure a uBlox series 8 device

 * ******************************/

// set rate of GPS to 5hz
const char UBLOX_INIT_5HZ[] PROGMEM = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};
// set rate of GPS to 10Hz
const char UBLOX_INIT_10HZ[] PROGMEM = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};
// set rate of GPS to 1Hz
const char UBLOX_INIT_1HZ[] PROGMEM = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xE8, 0x03, 0x01, 0x00, 0x01, 0x00, 0x01, 0x39};
// set rate of GPS to 1Hz
const char UBLOX_INIT_16HZ[] PROGMEM = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xE8, 0x03, 0x01, 0x00, 0x01, 0x00, 0x01, 0x39};

// GLL_ON
const char UBLOX_GxGLL_ON[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2F};
// GLL OFF
const char UBLOX_GxGLL_OFF[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};

// VTG ON
const char UBLOX_GxVTG_ON[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x4B};
// VTG OFF
const char UBLOX_GxVTG_OFF[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};

// Enable GxGSA (GNSS DOP and Active Satellites)
const char UBLOX_GxGSA_ON[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x05, 0x41};
// Disable GxGSA
const char UBLOX_GxGSA_OFF[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x31};
// Enable GxGSV (Sat View)
const char UBLOX_GxGSV_ON[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x06, 0x48};
// Disable GxGSV
const char UBLOX_GxGSV_OFF[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x38};
// set Max SVs per Talker id to 8
const char UBLOX_INIT_CHANNEL_8[] PROGMEM = {0xB5, 0x62, 0x06, 0x17, 0x14, 0x00, 0x00, 0x41, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0xE7};
// standard max SVs and extended digits for unsupported SVs
const char UBLOX_INIT_CHANNEL_ALL[] PROGMEM = {0xB5, 0x62, 0x06, 0x17, 0x14, 0x00, 0x00, 0x41, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x63};
// use GP as Main talker and GSV Talker ID - this is needed for TrackAddict
const char UBLOX_INIT_MAINTALKER_GP[] PROGMEM = {0xB5, 0x62, 0x06, 0x17, 0x14, 0x00, 0x00, 0x41, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78};
// use GP as Main talker and GSV Talker ID and restrict to GPS SVs ony - this is needed for RaceChrono
const char UBLOX_INIT_MAINTALKER_GP_GPSONLY[] PROGMEM = {0xB5, 0x62, 0x06, 0x17, 0x14, 0x00, 0x10, 0x41, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0xB8};
// set gps port BAUD rate
const char UBLOX_BAUD_57600[] PROGMEM = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0xE1, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xA9};
const char UBLOX_BAUD_38400[] PROGMEM = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x70};
const char UBLOX_BAUD_115200[] PROGMEM = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x5E};
// power saving
// enable power saving mode for 1800 or 3600 seconds
// UBX-RXM-PMREQ request
const char UBLOX_PWR_SAVE_30MIN[] PROGMEM = {0xB5, 0x62, 0x02, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x77, 0x1B, 0x00, 0x02, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x0F, 0xF6};
const char UBLOX_PWR_SAVE_1HR[] PROGMEM = {0xB5, 0x62, 0x02, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xEE, 0x36, 0x00, 0x02, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0xE1, 0x21};
const char UBLOX_PWR_OFF[] PROGMEM = { 0xB5, 0x62, 0x02, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x5D, 0x4B};
//, 0xrestart UBX-CFG-RST with controlled GNSS only software, hotstart (<4 hrs) so that ephemeris still valid
const char UBLOX_WARMSTART[] PROGMEM = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x68};
// Display precision in some apps
const char UBLOX_GxGBS_ON[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x09, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x0C, 0x72};
const char UBLOX_GxGBS_OFF[] PROGMEM = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x62};
// Poll GNGSA
const char UBLOX_GNGSA_POLL[] PROGMEM = "$EIGNQ,GSA*2D\r\n";
// Poll GNGSV
const char UBLOX_GNGSV_POLL[] PROGMEM = "$EIGNQ,GSV*3A\r\n";
// Poll GPGSA
const char UBLOX_GPGSA_POLL[] PROGMEM = "$EIGPQ,GSA*33\r\n";
// Poll GPGSV
const char UBLOX_GPGSV_POLL[] PROGMEM = "$EIGPQ,GSV*24\r\n";