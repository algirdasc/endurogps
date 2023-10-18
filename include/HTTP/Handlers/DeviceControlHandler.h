#include <WebServer.h>
#include "Device.h"
#include "HTTP/Template.h"

class DeviceControlHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri.startsWith("/device");
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.sendHeader("Location", "/", true);
            server.send(302, contentTypeHtml, "");

            if (requestUri.endsWith("/restart")) {
                ESP.restart();
            } else if (requestUri.endsWith("/poweroff")) {
                Device device;
                device.powerOff();
            }
            
            return true;
        }
};
