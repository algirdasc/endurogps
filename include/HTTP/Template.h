#include "HTML.h"
#include "EnduroGPS.h"
#include "Battery.h"
#include "WebServer.h"

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
        <link rel="stylesheet" href="/pure-css.css">
        <link rel="stylesheet" href="/style.css">
        </head>
    <body>
        <div id="layout">
            <a href="#" id="menuLink" class="menu-link">
                <span></span>
            </a>

            <div id="menu">
                <div class="pure-menu">
                    <a class="pure-menu-title" href="#company">EnduroGPS</a>

                    <ul class="pure-menu-list">
                        <li class="pure-menu-item">
                            <a href="/" class="pure-menu-link">Status</a>
                            <a href="/sdcard" class="pure-menu-link">Files</a>
                        </li>
                        <li class="pure-menu-heading menu-item-divided">Settings</li>
                        <li class="pure-menu-item">
                            <a href="/settings/wifi" class="pure-menu-link">Wireless</a>
                            <a href="/settings/gps" class="pure-menu-link">GPS</a>
                            <a href="/settings/firmware" class="pure-menu-link">Firmware</a>
                        </li>
                        <li class="pure-menu-heading menu-item-divided">Power</li>
                        <li class="pure-menu-item">
                            <a href="/device/restart" class="pure-menu-link" onclick="confirm('Are you sure want to restart device?')">Restart</a>
                            <a href="/device/poweroff" class="pure-menu-link" onclick="confirm('Are you sure want to power off device?')">Power off</a>
                        </li>
                    </ul>

                    <div class="menu-bottom">
                        <a href="https://github.com/)raw" GIT_REPO "\">" ENDUROGPS_VERSION R"raw(</a>
                    </div>
                </div>
            </div>

            <div id="main">                
    )raw";

const char WEBPORTAL_FOOTER[] PROGMEM = R"raw(</div></div></div></body></html>)raw";

const char contentTypeHtml[] PROGMEM = "text/html";
const char contentTypeCss[] PROGMEM = "text/css";
const char contentTypeStream[] PROGMEM = "application/octet-stream";

class Template
{
    public:
        static String generateBody(String content, String title = "", String subtitle = "") 
        {
            Battery battery;

            String htmlBody((char *) 0);

            htmlBody.reserve(WEBPORTAL_HEADER_STATIC_SIZE + WEBPORTAL_HEADER_DYN_SIZE + WEBPORTAL_FOOTER_STATIC_SIZE + content.length());
            htmlBody += String(WEBPORTAL_HEADER);

            if (title != "") {
                htmlBody += "<div class=\"header\">";
                htmlBody += "<h1>" + title + "</h1>";
                if (subtitle != "") {
                    htmlBody += "<h2>" + subtitle + "</h2>";
                }
                htmlBody += "</div>";                    
            }

            htmlBody += "<div class=\"content\">" + content + "</div>";
            htmlBody += String(battery.percentage());
            htmlBody += String(WEBPORTAL_FOOTER);

            return htmlBody;
        }

        static void redirect(WebServer &server, String uri)
        {
            server.sendHeader("Location", uri);
            server.send(302, contentTypeHtml, 
                Template::generateBody("<div class=\"text-center\">Redirecting to <a href=\"uri\">" + uri + "</a></div>")
            );
        }

};

#endif
