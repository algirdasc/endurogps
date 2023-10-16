#include "HTTP.h"
#include "HTTP/Template.h"
#include "HTTP/Handlers/CssHandler.h"
#include "HTTP/Handlers/IndexHandler.h"
#include "HTTP/Handlers/FileBrowserHandler.h"
#include "HTTP/Handlers/SettingsHandler.h"
#include "HTTP/Handlers/SettingsWifiHandler.h"
#include "HTTP/Handlers/SettingsFirmwareHandler.h"
#include "HTTP/Handlers/SettingsGPSHandler.h"

HTTP::HTTP(int port) 
{
    HTTP::port = port;
    WebServer server(HTTP::port);
}

void HTTP::start() 
{
    server.onNotFound(std::bind(&HTTP::notFound, this));

    server.addHandler(new CssHandler());
    server.addHandler(new IndexHandler());
        
    server.addHandler(new SettingsGPSHandler());        
    server.addHandler(new SettingsWifiHandler());
    server.addHandler(new SettingsFirmwareHandler());
    server.addHandler(new SettingsHandler());

    server.addHandler(new FileBrowserHandler());

    server.begin();

    log_d("Web server started on port %d", HTTP::port);
}

void HTTP::stop() 
{
    server.close();
}

void HTTP::handleClient() 
{
    server.handleClient();
}

// String HTTP::generateHeader(bool addMenu = true) 
// {
//     String htmlBody((char *) 0);

//     htmlBody.reserve(WEBPORTAL_HEADER_DYN_SIZE);

//     htmlBody += "<header>";

//     if (addMenu) {
//         htmlBody += F("Back to <a href='/'>Main menu</a>");
//     } else {
        
//     }

//     htmlBody += "</header>";

//     return htmlBody;
// }

// String HTTP::generateBody(String body, bool addMenu = true) 
// {
//     String htmlBody((char *) 0);

//     htmlBody.reserve(WEBPORTAL_HEADER_STATIC_SIZE + WEBPORTAL_HEADER_DYN_SIZE + WEBPORTAL_FOOTER_STATIC_SIZE + body.length());
//     htmlBody += String(WEBPORTAL_HEADER);
//     htmlBody += HTTP::generateHeader(addMenu);
//     htmlBody += body;
//     htmlBody += String(WEBPORTAL_FOOTER);

//     return htmlBody;
// }

// Core pages
void HTTP::notFound() 
{
    server.send(404, contentTypeHtml, 
            F("<h1> 404 Not found</h1><p>The requested resource was not found on this server.</p>")
    );
}
