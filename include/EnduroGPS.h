// Wifi Settings
#define ENDUROGPS_AP_SSID "EnduroGPS"
#define ENDUROGPS_AP_PASS "endurogps815"
#define ENDUROGPS_AP_MDNS "endurogps.local"

// HTTP
#define ENDUROGPS_HTTP_PORT 80

// Misc
#define ENDUROGPS_FIRMWARE_VER GIT_REV
#define ENDUROGPS_VERSION ENDUROGPS_FIRMWARE_VER " [" BUILD_ENV_NAME "]"

#define NMEA_SERVER_PORT 1818
#define NMEA_MIN_MESSAGE_SIZE 6

// HW
#define GPIO_BATTERY GPIO_NUM_35
#define GPIO_BUTTON GPIO_NUM_33

#define LOG_BAUD_RATE 115200

#define SerialMonitor Serial
#define GPSSerial Serial2
#define GPS_RX 5 
#define GPS_TX 18

// GPS
// How large should the RX buffer for the GPS port - more than 512 creates issues
#define UART_BUFFER_SIZE_RX 256
// Size of the intermediate buffer where we are storing the current sentence
#define MAX_UART_BUFFER_SIZE 256
#define GPS_UART_TIMEOUT 10000UL
#define GPS_STANDARD_BAUD_RATE 115200
