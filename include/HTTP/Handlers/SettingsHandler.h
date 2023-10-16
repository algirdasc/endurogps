#include <WebServer.h>
#include "HTTP/Template.h"

const char SETTINGS_PAGE_TEMPLATE[] PROGMEM = R"raw(
    <div class="row">
        <div class="col c4">
            <a href="/settings/wifi" class="smooth btn">WiFi</a>
        </div>
        <div class="col c4">
            <a href="/settings/gps" class="smooth btn">GPS</a>
        </div>
        <div class="col c4">
            <a href="/settings/firmware" class="smooth btn">Firmware</a>
        </div>
    </div>
    )raw";

class SettingsHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri == "/settings";
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.send(200, contentTypeHtml, Template::generateBody(SETTINGS_PAGE_TEMPLATE));

            return true;
        }
};