#pragma once

#include "HTML.h"
#include "EnduroGPS.h"
#include "Battery.h"
#include "WebServer.h"
#include "HTTP/Assets/HTML/Header.h"
#include "HTTP/Assets/HTML/Footer.h"

#define WEBPORTAL_HEADER_STATIC_SIZE 350
#define WEBPORTAL_HEADER_DYN_SIZE 150
#define WEBPORTAL_FOOTER_STATIC_SIZE 210

const char contentTypeHtml[] PROGMEM = "text/html";
const char contentTypeCss[] PROGMEM = "text/css";
const char contentTypeStream[] PROGMEM = "application/octet-stream";

class Template
{
    public:
        static String generateBody(String content, String title = "", String subtitle = "") 
        {            
            String htmlBody((char *) 0);

            htmlBody.reserve(WEBPORTAL_HEADER_STATIC_SIZE + WEBPORTAL_HEADER_DYN_SIZE + WEBPORTAL_FOOTER_STATIC_SIZE + content.length());
            htmlBody += String(HTML_HEADER);

            if (title != "") {
                htmlBody += "<div class=\"header\">";
                htmlBody += "<h1>" + title + "</h1>";
                if (subtitle != "") {
                    htmlBody += "<h2>" + subtitle + "</h2>";
                }
                htmlBody += "</div>";                    
            }

            htmlBody += "<div class=\"content\">" + content + "</div>";
            htmlBody += String(batteryContainer());
            htmlBody += String(HTML_FOOTER);

            return htmlBody;
        }

        static void redirect(WebServer &server, String uri)
        {
            server.sendHeader("Location", uri);
            server.send(302, contentTypeHtml, 
                Template::generateBody("<div class=\"text-center\">Redirecting to <a href=\"" + uri + "\">" + uri + "</a></div>")
            );
        }

        static String batteryContainer()
        {
            Battery battery;

            uint batteryPercentage = battery.percentage();
            uint batteryIconClass = (6 * batteryPercentage) / 100;

            return R"raw(<div id="battery"><span class="icon battery-)raw" + String(batteryIconClass) + R"raw("></span> <span id="battery_percent">)raw" + String(batteryPercentage) + R"raw(%</span></div>)raw";
        }

};
