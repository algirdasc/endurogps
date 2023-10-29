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
#define GPIO_BATTERY        GPIO_NUM_35
#define GPIO_BUTTON         GPIO_NUM_33
#define GPIO_GPS_RX         GPIO_NUM_13
#define GPIO_GPS_TX         GPIO_NUM_15

// Serial
#define LOG_BAUD_RATE       115200
#define GPS_BAUD_RATE       115200
#define GPS_RX_BUFFER_SIZE  256 // How large should the RX buffer for the GPS port - more than 512 creates issues
#define GPS_MAX_BUFFER_SIZE 256 // Size of the intermediate buffer where we are storing the current sentence
#define GPS_UART_TIMEOUT    5000UL //

#define SerialMonitor       Serial
#define GPSSerial           Serial2

// Misc
#define UPDATE_FILE_PATH    "/firmware.bin"
#define UPDATE_FILE_URL     "https://tools.cici.lt/update.php"