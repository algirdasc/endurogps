#include "HTTP.h"
#include "HTTP/Template.h"
#include "HTTP/Handlers/CssHandler.h"
#include "HTTP/Handlers/IndexHandler.h"
#include "HTTP/Handlers/FileBrowserHandler.h"
#include "HTTP/Handlers/SettingsHandler.h"

HTTP::HTTP(int port) 
{
    HTTP::port = port;
    WebServer web(HTTP::port);
}

void HTTP::start() 
{
    // Core
    web.onNotFound(std::bind(&HTTP::notFound, this));

    web.addHandler(new CssHandler());
    web.addHandler(new IndexHandler());
    web.addHandler(new SettingsHandler());
    web.addHandler(new FileBrowserHandler());

    // Serve!
    web.begin();

    log_d("Web server started on port %d", HTTP::port);
}

void HTTP::stop() 
{
    web.close();
}

void HTTP::handleClient() 
{
    web.handleClient();
}

String HTTP::generateHeader(bool addMenu = true) 
{
    String htmlBody((char *) 0);

    htmlBody.reserve(WEBPORTAL_HEADER_DYN_SIZE);

    htmlBody += "<header>";

    if (addMenu) {
        htmlBody += F("Back to <a href='/'>Main menu</a>");
    } else {
        
    }

    htmlBody += "</header>";

    return htmlBody;
}

String HTTP::generateBody(String body, bool addMenu = true) 
{
    String htmlBody((char *) 0);

    htmlBody.reserve(WEBPORTAL_HEADER_STATIC_SIZE + WEBPORTAL_HEADER_DYN_SIZE + WEBPORTAL_FOOTER_STATIC_SIZE + body.length());
    htmlBody += String(WEBPORTAL_HEADER);
    htmlBody += HTTP::generateHeader(addMenu);
    htmlBody += body;
    htmlBody += String(WEBPORTAL_FOOTER);

    return htmlBody;
}

// Core pages
void HTTP::notFound() 
{
    web.send(
        404, 
        html_text, 
        generateBody(
            F("<h1> 404 Not found</h1><p>The requested resource was not found on this server.</p>")
        )
    );
}
