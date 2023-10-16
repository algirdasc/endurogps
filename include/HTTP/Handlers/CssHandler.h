#include <WebServer.h>

const char WEBPORTAL_CSS[] PROGMEM = ""
    "*{"
    "box-sizing:border-box;"
    "text-align:center;"
    "width:100%;"
    "font-weight:300;"
    "}"
    "body{"
    "font-family:Roboto,system-ui,Arial,Helvetica,sans-serif;"
    "font-size:5vw;"
    "margin:0"
    "}"
    "header{"
    "background-color:#666;"
    "padding:.5vw;text-align:center;color:#fff}article{float:left;padding:10px;width:100%;height:auto}details{display:table;clear:both}summary{font-size:larger;font-weight:400;padding:10px;background-color:#f1f1f1}footer{background-color:#777;padding:.2vw;text-align:center;color:#fff;clear:both;position:fixed;bottom:0;font-size:small}@media(min-width:800px){article{width:50%}*{font-size:2.5vw}.bat{height:auto;max-height:2vw}}a,input{color:#fff;border-radius:8pt;background:red;text-decoration:none;padding:5pt}.bg input{display:none}label{border:solid;border-radius:8pt;padding:5px;margin:2px;border-color:#bdbdbd;border-width:2px;color:#9e9e9e}.bg input:checked+label,.bg input:checked+label:active{background:red;color:#fff;border-color:red}.bat{position:relative;width:8vw;height:3vw;border:1px solid #fff;border-radius:2px;position:relative;padding:1px;display:inline-block}.bat:before{content:'';display:block;position:absolute;top:50%;right:-5.1px;margin-top:-3px;width:6px;height:6px;background:#fff;clip:rect(0,6px,6px,3.9px);border-radius:50%}.bat .lvl{height:100%;background:#fff;border-radius:1px}";


class CssHandler : public RequestHandler
{
    public:
        bool canHandle(HTTPMethod requestMethod, String requestUri)
        {
            return requestMethod == HTTP_GET && requestUri == "/style.css";
        }

        bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri)
        {
            server.sendHeader("Cache-Control", "max-age=86400");
            server.send_P(200, "text/css", WEBPORTAL_CSS);   

            return true;
        }
};
