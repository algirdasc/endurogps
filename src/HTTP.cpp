#include "HTTP.h"
#include "HTTP/Template.h"
#include "HTTP/Handlers/AssetHandler.h"
#include "HTTP/Handlers/IndexHandler.h"
#include "HTTP/Handlers/FileBrowserHandler.h"
#include "HTTP/Handlers/SettingsWifiHandler.h"
#include "HTTP/Handlers/SettingsFirmwareHandler.h"
#include "HTTP/Handlers/SettingsGPSHandler.h"

HTTP::HTTP() 
{
    WebServer server;
}

void HTTP::start() 
{
    server.onNotFound(std::bind(&HTTP::notFound, this));
    server.on("/device/please_wait", std::bind(&HTTP::pleaseWait, this));

    server.addHandler(new AssetHandler());
    server.addHandler(new IndexHandler());
    server.addHandler(new SettingsGPSHandler());        
    server.addHandler(new SettingsWifiHandler());
    server.addHandler(new SettingsFirmwareHandler());
    server.addHandler(new FileBrowserHandler());

    server.begin(HTTP_PORT);

    log_d("Web server started on port %d", HTTP_PORT);
}

void HTTP::on(const Uri &uri, THandlerFunction fn)
{
    server.on(uri, fn);
}

void HTTP::stop() 
{
    server.close();
}

void HTTP::handleClient() 
{
    server.handleClient();
}

void HTTP::notFound() 
{
    server.send(404, contentTypeHtml, 
        Template::generateBody("<h1> 404 Not found</h1><p>The requested resource was not found on this server.</p>")
    );
}

void HTTP::redirect(String uri)
{
    server.sendHeader("Location", uri);
    server.send(302, contentTypeHtml, 
        Template::generateBody("<div class=\"text-center\">Redirecting to <a href=\"uri\">" + uri + "</a></div>")
    );
}

void HTTP::pleaseWait()
{
    server.send(200, contentTypeHtml, 
        Template::generateBody("", "Please wait", "Device is executing your command")
    );
}
