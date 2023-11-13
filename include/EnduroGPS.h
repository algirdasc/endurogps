// Wifi Settings
#define DEVICE_NAME         "EnduroGPS"
#define WIFI_AP_MDNS        "endurogps.local"

// Ports
#define HTTP_PORT           80
#define NMEA_PORT           1818

// Misc
#define FIRMWARE            GIT_REV
#define VERSION             FIRMWARE " [" BUILD_ENV_NAME "]"

// HW
#ifdef ARDUINO_TTGO_T1

#define GPIO_GPS_RX         GPIO_NUM_17
#define GPIO_GPS_TX         GPIO_NUM_16
#else
#define GPIO_GPS_RX         GPIO_NUM_13
#define GPIO_GPS_TX         GPIO_NUM_15
#endif

#define GPIO_LED            LED_BUILTIN
#define GPIO_BATTERY        GPIO_NUM_35
#define GPIO_BUTTON         GPIO_NUM_12


// Serial
#define LOG_BAUD_RATE       115200
#define GPS_BAUD_RATE       115200
#define GPS_RX_BUFFER_SIZE  256 // How large should the RX buffer for the GPS port - more than 512 creates issues

#define SerialMonitor       Serial
#define GPSSerial           Serial2

// Misc
#define UPDATE_FILE_PATH    "/firmware.bin"