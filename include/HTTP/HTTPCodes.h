#pragma once

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_FOUND 404
#define HTTP_CODE_REDIRECT 302

const char *responseCode(uint code)
{
    switch (code)
    {
    case 200:
        return "OK";
    case 302:
        return "Found";
    case 404:
        return "Not Found";
    default:
        return "";
    }
}