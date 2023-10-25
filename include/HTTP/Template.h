#pragma once

#include "HTML.h"
#include "EnduroGPS.h"
#include "Battery.h"
#include "WebServer.h"
#include "HTTP/HTTPCodes.h"
#include "HTTP/Assets/HTML/Header.h"
#include "HTTP/Assets/HTML/Footer.h"

#define TEMPLATE_HEADER_STATIC_SIZE 2048
#define TEMPLATE_HEADER_DYN_SIZE 128
#define TEMPLATE_FOOTER_STATIC_SIZE 64

const char contentTypeHtml[] PROGMEM = "text/html";
const char contentTypeCss[] PROGMEM = "text/css";
const char contentTypeStream[] PROGMEM = "application/octet-stream";

const char TEMPLATE_PAGE_TITLE[] PROGMEM = R"raw(<div class="page-header"><h1>%s</h1></div>%s)raw";
const char TEMPLATE_PAGE_SUBTITLE[] PROGMEM = R"raw(<p class="lead text-muted">%s</p>)raw";

class Template
{
public:
    static String generateBody(String content, String title = "", String subtitle = "")
    {
        String htmlBody((char *)0);

        htmlBody.reserve(TEMPLATE_HEADER_STATIC_SIZE + TEMPLATE_HEADER_DYN_SIZE + TEMPLATE_FOOTER_STATIC_SIZE + content.length());
        htmlBody += String(HTML_HEADER);

        // if (title != "") {
        //     char title[128];
        //     char subtitle[128];

        //     if (subtitle != "") {
        //         sprintf(subtitle, TEMPLATE_PAGE_SUBTITLE, subtitle);
        //     }

        //     sprintf(title, TEMPLATE_PAGE_TITLE, title, subtitle);

        //     htmlBody += title;
        // }

        htmlBody += content;
        htmlBody += String(batteryContainer());
        htmlBody += String(HTML_FOOTER);

        return htmlBody;
    }

    static void redirect(WebServer &server, String uri)
    {
        server.sendHeader("Location", uri);
        server.send(HTTP_CODE_REDIRECT, contentTypeHtml,
                    Template::generateBody("<div class=\"text-center\">Redirecting to <a href=\"" + uri + "\">" + uri + "</a></div>"));
    }

    static String batteryContainer()
    {
        Battery battery;

        uint batteryPercentage = battery.percentage();
        uint batteryIconClass = (6 * batteryPercentage) / 100;

        return R"raw(<div id="battery"><span class="icon battery-)raw" + String(batteryIconClass) + R"raw("></span> <span id="battery_percent">)raw" + String(batteryPercentage) + R"raw(%</span></div>)raw";
    }
};
