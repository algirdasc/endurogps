#pragma once

#include <SD.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include "HTTP/URL.h"
#include "HTTP/Template.h"


const char FILEBROWSER_PAGE_BASE_URL[] PROGMEM = "/sdcard";
const char FILEBROWSER_PAGE_TITLE[] PROGMEM = "File browser";
const char FILEBROWSER_PAGE_404_ERROR[] PROGMEM = "SD Card is unavailable or path does not exist";
const char FILEBROWSER_PAGE_TABLE_HEADER_TEMPLATE[] PROGMEM = R"raw(
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
const char FILEBROWSER_PAGE_TABLE_FOOTER_TEMPLATE[] PROGMEM = R"raw(
        </tbody>
    </table>
    </div></div>
)raw";
const char FILEBROWSER_PAGE_DIRECTORY_EMPTY_ROW[] PROGMEM = R"raw(
    <tr>
        <td class="text-center" colspan="100">
            Directory is empty
        </td>
    </tr>
)raw";


class FileBrowserHandler : public RequestHandler
{

    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestUri.startsWith(FILEBROWSER_PAGE_BASE_URL);
        }

        bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri)
        {            
            String sdCardPath = server.urlDecode(requestUri);
            sdCardPath.remove(0, 8);
            if (!sdCardPath.startsWith("/")) {
                sdCardPath = "/" + sdCardPath;
            }

            if (!SD.exists(sdCardPath)) {
                server.send(HTTP_CODE_NOT_FOUND, contentTypeHtml, 
                    Template::generateBody("", FILEBROWSER_PAGE_TITLE, FILEBROWSER_PAGE_404_ERROR)
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
                server.send(HTTP_CODE_OK, contentTypeHtml, 
                    Template::generateBody(directoryHtmlTable(entry), FILEBROWSER_PAGE_TITLE, entry.path())
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
            return String(FILEBROWSER_PAGE_BASE_URL) + path;
        }

        String parentDir(String path) 
        {
            int index = path.lastIndexOf("/");

            return path.substring(0, index);
        }

        String directoryHtmlTable(File dir)
        {            
            String parentUrl = absoluteUrl(parentDir(dir.path()));
            String table = FILEBROWSER_PAGE_TABLE_HEADER_TEMPLATE;

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
                        table += FILEBROWSER_PAGE_DIRECTORY_EMPTY_ROW;
                    }

                    break;                    
                }            

                fileCount++;    

                // TODO: optimize icon html
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

            table += FILEBROWSER_PAGE_TABLE_FOOTER_TEMPLATE;

            return table;
        }
};
