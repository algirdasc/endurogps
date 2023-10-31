#pragma once

#include <SD.h>
#include <WebServer.h>

#include "HTTP.h"

const char FILEBROWSER_PAGE_BASE_URL[] = "/sdcard";
const char FILEBROWSER_PAGE_TABLE_HEADER_TEMPLATE[] = R"raw(<table class="table table-striped"><thead><tr><th></th><th>Filename</th><th class="text-center">Size</th><th class="text-right">Actions</th></tr></thead><tbody>)raw";
const char FILEBROWSER_PAGE_TABLE_FOOTER_TEMPLATE[] = R"raw(</tbody><tfoot><tr><th colspan="100" class="text-right text-muted">Used: %llu MB / %llu MB</th></tr></tfoot></table></div></div>)raw";
const char FILEBROWSER_PAGE_TABLE_EMPTY_DIR_ROW[] = R"raw(<tr><td class="text-center" colspan="100">Directory is empty</td></tr>)raw";
const char FILEBROWSER_PAGE_TABLE_SUBDIR_ROW[] = R"raw(<tr><td><a href="%s"><span class="icon folder-open"></span></a></td><td colspan="100"><a href="%s">Back</a></td></tr>)raw";
const char FILEBROWSER_PAGE_TABLE_ROW[] = R"raw(<tr><td>[%s]</td><td><a href="%s">%s</a></td><td class="text-center">%s</td><td class="text-right"><a href="%s?delete=1" class="btn btn-xs btn-danger" onclick="confirm('Are you sure you want to delete this item?');">Delete</a></td></tr>)raw";

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
        if (!sdCardPath.startsWith("/"))
        {
            sdCardPath = "/" + sdCardPath;
        }

        if (!SD.begin()) 
        {
            displayError(server, "Failed to mount SD card!");

            return true;
        }

        if (!SD.exists(sdCardPath))
        {
            displayError(server, "Path does not exist!");

            return true;
        }

        File entry = SD.open(sdCardPath);
        if (server.hasArg("delete"))
        {
            entry.isDirectory() ? SD.rmdir(sdCardPath) : SD.remove(sdCardPath);

            HTTP::redirect(server, relativeUrl(parentDir(entry.path())));

            return true;
        }

        if (entry.isDirectory())
        {
            displayDirectoryHtmlTable(server, entry);
        }
        else
        {
            size_t sent = server.streamFile(entry, "application/stream");
        }

        entry.close();

        return true;
    }

private:
    const char *bytesToHumanReadable(size_t size)
    {
        const char *suffix[5] = {"B", "KB", "MB", "GB", "TB"};

        int i = 0;
        double dblBytes = size;
        if (size > 1024)
        {
            for (i = 0; (size / 1024) > 0 && i < 4; i++, size /= 1024)
            {
                dblBytes = size / 1024.0;
            }
        }

        FixedString16 output;
        output.appendFormat("%.02lf %s", dblBytes, suffix[i]);

        return output.c_str();
    }

    const char *relativeUrl(const char *path)
    {
        FixedString128 relativeUrl;
        relativeUrl.appendFormat("%s%s", FILEBROWSER_PAGE_BASE_URL, path);

        return relativeUrl.c_str();
    }

    const char *parentDir(const char *path)
    {
        char parentPath[128] = "";
        uint lastSlash = strrchr(path, '/') - path;
        if (lastSlash == 0)
        {
            lastSlash++;
        }
        strncat(parentPath, path, lastSlash);

        FixedString128 parentDir = parentPath;

        return parentDir.c_str();
    }

    void displayError(WebServer &server, const char *error)
    {
        server.sendContent(HTML_HEADER);
        
        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "File Browser");

        server.sendContent(pageHeader.c_str());
        
        FixedString256 errorMessage;
        errorMessage.appendFormat(HTML_ALERT, error);

        server.sendContent(errorMessage.c_str());
        server.sendContent(HTML::js());
        server.sendContent(HTML_FOOTER);
    }

    void displayDirectoryHtmlTable(WebServer &server, File dir)
    {
        FixedString128 parentUrl = relativeUrl(parentDir(dir.path()));

        server.sendContent(HTML_HEADER);
        
        FixedString64 pageHeader;
        pageHeader.appendFormat(HTML_PAGE_HEADER, "File Browser");

        server.sendContent(pageHeader.c_str());

        server.sendContent(R"raw(<ol class="breadcrumb">)raw");
        server.sendContent(HTML::breadcrumb(FILEBROWSER_PAGE_BASE_URL, "Home", false));

        char *pch = strtok((char *)dir.path(), "/");
        while (pch != NULL)
        {
            server.sendContent(HTML::breadcrumb(relativeUrl(pch), pch, false));
            pch = strtok(NULL, "/");
        }

        server.sendContent("</ol>");
        server.sendContent(FILEBROWSER_PAGE_TABLE_HEADER_TEMPLATE);

        if (strcmp(dir.path(), "/") != 0)
        {
            FixedString512 tableRow;
            tableRow.appendFormat(FILEBROWSER_PAGE_TABLE_SUBDIR_ROW, parentUrl.c_str(), parentUrl.c_str());

            server.sendContent(tableRow.c_str());
        }

        uint fileCount = 0;
        while (true)
        {
            File entry = dir.openNextFile();
            if (!entry)
            {
                if (fileCount == 0)
                {
                    server.sendContent(FILEBROWSER_PAGE_TABLE_EMPTY_DIR_ROW);
                }

                break;
            }

            const char *entryRelativeUrl = relativeUrl(entry.path());

            FixedString512 tableRow;
            tableRow.appendFormat(FILEBROWSER_PAGE_TABLE_ROW,
                                  entry.isDirectory() ? "DIR" : "FILE",
                                  entryRelativeUrl,
                                  entry.name(),
                                  entry.isDirectory() ? "" : bytesToHumanReadable(entry.size()),
                                  entryRelativeUrl);

            server.sendContent(tableRow.c_str());

            entry.close();
            fileCount++;
        }

        FixedString256 tableFooter;
        tableFooter.appendFormat(FILEBROWSER_PAGE_TABLE_FOOTER_TEMPLATE, SD.usedBytes() / 1048576, SD.cardSize() / 1048576);
        server.sendContent(tableFooter.c_str());
        server.sendContent(HTML::js());
        server.sendContent(HTML_FOOTER);
    }
};
