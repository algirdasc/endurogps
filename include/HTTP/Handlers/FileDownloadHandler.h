#include <SD.h>
#include <WebServer.h>

class FileDownloadHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && requestUri.startsWith("/sd/download");
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            String sdCardPath = requestUri.substring(3);

            if (SD.exists(sdCardPath)) {
                File file = SD.open(sdCardPath, "r");
                size_t sent = server.streamFile(file, "application/octet-stream");
                file.close();

                return true;
            }

            return false;
        }
};
