#include "EnduroGPS.h"

#define WEBPORTAL_HEADER_STATIC_SIZE 350
#define WEBPORTAL_HEADER_DYN_SIZE 150
#define WEBPORTAL_FOOTER_STATIC_SIZE 210

#ifndef WEBPORTAL_HEADER
const char WEBPORTAL_HEADER[] PROGMEM = ""
    "<!DOCTYPE html>\n"
    "<html lang='en'>\n"
    "\t<head>\n"
    "\t\t<title>EnduroGPS</title>\n"
    "\t\t<meta charset='utf-8'>\n"
    "\t\t<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
    "\t\t<link rel='stylesheet' href='/style.css'>\n"
    "\t</head>\n"
    "<body>\n"
    ;
#endif

const char WEBPORTAL_FOOTER[] PROGMEM = ""
    "<footer>"
    "Version: "
    "<a style='font-size: small;background: none;text-decoration: underline;' target='_blank' href='https://github.com/" GIT_REPO "'>" ENDUROGPS_VERSION "</a>"
    "</footer>\n"
    "</body>\n"
    "</html>"
    ;

const char html_text[] PROGMEM = "text/html";
const char html_css[] PROGMEM = "text/css";
const char text_json[] PROGMEM = "application/json";
const char json_ok[] PROGMEM = "{'status':'ok'}";
const char json_error[] PROGMEM = "{'status':'error'}";