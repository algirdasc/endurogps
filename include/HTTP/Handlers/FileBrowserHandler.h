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

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            String sdCardPath = String(requestUri);
            sdCardPath.remove(0, 8);
            sdCardPath = ensureRootPrefix(sdCardPath);

            log_d("sdCardPath: %s", sdCardPath);

            if (!SD.exists(sdCardPath)) {
                server.send(404, contentTypeHtml, 
                    Template::generateBody("", "SD Card error", "SD Card is unavailable or path does not exist")
                );

                return true;
            }

            File entry = SD.open(sdCardPath);
            if (server.hasArg("delete")) {
                entry.isDirectory() ? SD.rmdir(sdCardPath) : SD.remove(sdCardPath);

                Template::redirect(server, parentDir(sdCardPath));

                return true;
            }

            if (entry.isDirectory()) {
                server.send(200, contentTypeHtml, 
                    Template::generateBody(directoryHtmlTable(entry), "File browser: " + sdCardPath)
                );
            } else {
                size_t sent = server.streamFile(entry, contentTypeStream);                
            }

            entry.close();
            
            return true;
        }

    private:
        char* bytesToHumanReadable(size_t size)
        {
            char *suffix[5] = {"B", "KB", "MB", "GB", "TB"};

            int i = 0;
	        double dblBytes = size;
        	if (size > 1024) {
		        for (i = 0; (size / 1024) > 0 && i < 4; i++, size /= 1024) {
			        dblBytes = size / 1024.0;
                }
	        }

            static char output[128];
	        sprintf(output, "%.02lf %s", dblBytes, suffix[i]);

            return output;
        }

        String parentDir(String path) {
            int index = path.lastIndexOf("/");

            return ensureRootPrefix(path.substring(0, index));
        }

        String joinPath(String path, String file)
        {
            if (!path.endsWith("/")) {
                return path + "/" + file;
            }

            return path + file;
        }

        String ensureRootPrefix(String path)
        {
            if (!path.startsWith("/")) {
                path = "/" + path;
            }

            return path;
        }

        String directoryHtmlTable(File dir)
        {
            log_d("Directory %s", dir.path());

            String baseUri = joinPath(String(URI), dir.path());
            String table = R"raw(
                <div class="pure-g">
                    <div class="pure-u-1-1">
                        <table class="pure-table pure-table-horizontal pure-table-stretch">
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

            table += "<tr><td colspan='100'><a href='" + parentDir(baseUri) +"'>../</a></td></tr>";

            while (true) {
                File entry = dir.openNextFile();
                if (!entry) {
                    break;
                }                

                log_d("Entry: %s", entry.name());

                String entryUri = joinPath(baseUri, entry.name());

                String icon = "FILE";
                if (entry.isDirectory()) {
                    icon = "DIR";
                }

                table += "<tr>";
                table += "<td>" + icon + "</td>";
                table += "<td><a href=\"" + entryUri + "\">";
                table += entry.name();
                table += "</a></td>";

                table += "<td><span>";
                table += bytesToHumanReadable(entry.size());
                table += "</span></td>";

                table += "<td><a href=\"" + entryUri + "?delete=1\" onclick=\"confirm('Are you sure you want to delete this item?');\">";
                table += "Delete";
                table += "</a></td>";

                table += "</tr>";

                entry.close();                        
            }
            table += "</tbody></table></div></div>";

            return table;
        }
};
