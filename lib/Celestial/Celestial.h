#ifndef CELESTIAL_H
#define CELESTIAL_H

#include <Windows.h>
#include <winhttp.h>
#include <string>

#pragma comment(lib, "winhttp.lib")

class HttpRequest {
public:
    HttpRequest();
    ~HttpRequest();

    bool sendGetRequest(const std::wstring& url, std::wstring& response);
    bool sendGetRequestWithUserAgent(const std::wstring& url, const std::wstring& userAgent, std::wstring& response);
private:
    HINTERNET hSession;
    HINTERNET hConnect;
};


#endif // CELESTIAL_H
