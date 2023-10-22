#pragma once

#include <SD.h>
#include <WebServer.h>

#include "HTTP/URL.h"
#include "HTTP/Template.h"

const char* BASE_URL = "/sdcard";

const char FILE_BROWSER_TABLE_HEADER_TEMPLATE[] PROGMEM = R"raw(
    <table class="pure-table pure-table-horizontal pure-table-stretch">
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
            return requestUri.startsWith(BASE_URL);
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            String sdCardPath = server.urlDecode(requestUri);
            sdCardPath.remove(0, 8);
            if (!sdCardPath.startsWith("/")) {
                sdCardPath = "/" + sdCardPath;
            }

            if (!SD.exists(sdCardPath)) {
                server.send(404, contentTypeHtml, 
                    Template::generateBody("", "SD Card error", "SD Card is unavailable or path does not exist")
                );

                return true;
            }

            File entry = SD.open(sdCardPath);
            if (server.hasArg("delete")) {
                entry.isDirectory()? SD.rmdir(sdCardPath) : SD.remove(sdCardPath);

                Template::redirect(server, absoluteUrl(parentDir(entry.path())));

                return true;
            }

            if (entry.isDirectory()) {
                log_d("Entry: %s", entry.path());
                server.send(200, contentTypeHtml, 
                    Template::generateBody(directoryHtmlTable(entry), "File browser", entry.path())
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
            const char *suffix[5] = {"B", "KB", "MB", "GB", "TB"};

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

        String absoluteUrl(String path)
        {
            return String(BASE_URL) + path;
        }

        String parentDir(String path) 
        {
            int index = path.lastIndexOf("/");

            return path.substring(0, index);
        }

        String directoryHtmlTable(File dir)
        {            
            String parentUrl = absoluteUrl(parentDir(dir.path()));
            String table = FILE_BROWSER_TABLE_HEADER_TEMPLATE;

            if (!String(dir.path()).equalsIgnoreCase("/")) {
                table += R"raw(
                    <tr>
                        <td>                    
                            <a href=")raw" + parentUrl + R"raw("><span class="icon folder-open"></span></a>
                        </td>
                        <td colspan="100">
                            <a href=")raw" + parentUrl + R"raw(">Back</a>
                        </td>
                    </tr>)raw";
            }

            uint fileCount = 0;
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) {
                    if (fileCount == 0) {
                        table += R"raw(
                            <tr>
                                <td class="text-center" colspan="100">
                                    Directory is empty
                                </td>
                            </tr>
                        )raw";
                    }

                    break;                    
                }            

                fileCount++;    

                String icon = R"raw(<span class="icon file"></span>)raw";
                if (entry.isDirectory()) {
                    icon = R"raw(<span class="icon folder"></span>)raw";
                }

                String entryAbsoluteUrl = absoluteUrl(entry.path());
                                
                table += "<tr>";
                table += "<td>" + icon + "</td>";
                table += "<td><a href=\"" + entryAbsoluteUrl + "\">";
                table += entry.name();
                table += "</a></td>";

                table += "<td><span>";
                table += entry.isDirectory() ? "" : bytesToHumanReadable(entry.size());
                table += "</span></td>";

                table += "<td class=\"text-center\"><a href=\"" + entryAbsoluteUrl + "?delete=1\" onclick=\"confirm('Are you sure you want to delete this item?');\">";
                table += "<span class=\"icon delete\"></span>";
                table += "</a></td>";

                table += "</tr>";

                entry.close();                        
            }

            table += "</tbody></table></div></div>";

            return table;
        }
};
