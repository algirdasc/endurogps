#include <SD.h>
#include <WebServer.h>
#include "HTTP/Template.h"

const char* URI = "/sdcard";

const char FILE_BROWSER_TABLE_HEADER_TEMPLATE[] PROGMEM = R"raw(
        <thead>
            <tr>
                <th></th>
                <th>Filename</th>
                <th>Size</th>
                <th>Actions</th>
            </tr>
        </thead>
    )raw";


class FileBrowserHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri.startsWith(URI);
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            requestUri.replace(URI, "");
            if (!requestUri.startsWith("/")) {
                requestUri = "/" + requestUri;
            }

            if (!SD.exists(requestUri)) {
                log_d("File %s does not exist", requestUri);

                return false;
            }

            File entry = SD.open(requestUri);
            if (entry.isDirectory()) {
                server.send(200, contentTypeHtml, Template::generateBody(directoryHtmlTable(entry)));
            } else {
                size_t sent = server.streamFile(entry, contentTypeStream);                
            }

            entry.close();
            
            return true;
        }

    private:
        String directoryHtmlTable(File dir)
        {
            String baseUri = String(URI) + dir.path() + "/";
            String table = R"raw(
                <table class="table">
                    <thead>
                        <tr>
                            <th></th>
                            <th>Filename</th>
                            <th>Size</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody>
            )raw";

            table += "<tr><td colspan='100'><a href='" + baseUri +"'>../</a></td></tr>";

            while (true) {
                File entry = dir.openNextFile();
                if (!entry) {
                    break;
                }

                String entryUri = baseUri + entry.name();

                table += "<tr>";
                table += "<td></td>";
                table += "<td><a href='" + entryUri + "'>";
                table += entry.name();
                table += "</a></td>";

                table += "<td><span>";
                table += entry.size();
                table += "</span></td>";

                table += "<td>""<a href='#'>";
                table += "Delete";
                table += "</a></td>";

                table += "</tr>";

                entry.close();                        
            }
            table += "</tbody></table>";

            return table;
        }
};
