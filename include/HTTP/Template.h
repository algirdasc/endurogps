#include "EnduroGPS.h"

#ifndef TEMPLATE_H
#define TEMPLATE_H

#define WEBPORTAL_HEADER_STATIC_SIZE 350
#define WEBPORTAL_HEADER_DYN_SIZE 150
#define WEBPORTAL_FOOTER_STATIC_SIZE 210

const char WEBPORTAL_HEADER[] PROGMEM = R"raw(
    <!DOCTYPE html>
    <html lang="en">
        <head>
        <title>EnduroGPS</title>
        <meta charset="utf-8">
        <meta name="viewport' content='width=device-width, initial-scale=1">
        <link rel="stylesheet" href="/style.css">
        </head>
    <body>
        <nav class="nav" onclick="this.focus()">
            <div class="container">
                <a class="pagename current" href="#">EnduroGPS</a>
                <a href="/">Status</a>
                <a href="/sdcard">Files</a>
                <a href="/settings">Settings</a>                
            </div>
        </nav>
        <button class="btn-close btn btn-sm">X</button>
    )raw";

const char WEBPORTAL_FOOTER[] PROGMEM = ""
    "<footer>"
    "Version: "
    "<a style='font-size: small;background: none;text-decoration: underline;' target='_blank' href='https://github.com/" GIT_REPO "'>" ENDUROGPS_VERSION "</a>"
    "</footer>\n"
    "</body>\n"
    "</html>"
    ;

const char contentTypeHtml[] PROGMEM = "text/html";
const char contentTypeCss[] PROGMEM = "text/css";
const char contentTypeStream[] PROGMEM = "application/octet-stream";

class Template
{
    public:
        static String generateBody(String content) 
        {
            String htmlBody((char *) 0);

            htmlBody.reserve(WEBPORTAL_HEADER_STATIC_SIZE + WEBPORTAL_HEADER_DYN_SIZE + WEBPORTAL_FOOTER_STATIC_SIZE + content.length());
            htmlBody += String(WEBPORTAL_HEADER);
            //htmlBody += HTTP::generateHeader(addMenu);
            htmlBody += content;
            htmlBody += String(WEBPORTAL_FOOTER);

            return htmlBody;
        }
};

#endif