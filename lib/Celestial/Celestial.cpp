#define SECURITY_WIN32

#include "Celestial.h"
#include <security.h>
#include <iostream>
#include <sspi.h>

HttpRequest::HttpRequest() {
    // Initialize security package.
    PSecurityFunctionTableW securityFunctionTable;
    securityFunctionTable = InitSecurityInterfaceW();
    if (!securityFunctionTable) {
        std::cerr << "Failed to initialize security package." << std::endl;
    }

    // Initialize WinHTTP session.
    hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_SECURE);
}


HttpRequest::~HttpRequest() {

    
    if (hSession) {
        WinHttpCloseHandle(hSession);
    }
}

bool HttpRequest::sendGetRequest(const std::wstring& url, std::wstring& response) {
    return sendGetRequestWithUserAgent(url, L"WinHTTP Example Program", response);
}

bool HttpRequest::sendGetRequestWithUserAgent(const std::wstring& url, const std::wstring& userAgent, std::wstring& response) {
    if (!hSession) {
        std::cerr << "Failed to open WinHTTP session\n";
        return false;
    }

    // Connect to the server.
    hConnect = WinHttpConnect(hSession, url.c_str(),
        INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (!hConnect) {
        std::cerr << "Failed to connect to server\n";
        return false;
    }

    // Create HTTP request.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET",
        NULL, NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    if (!hRequest) {
        std::cerr << "Failed to open HTTP request\n";
        WinHttpCloseHandle(hConnect);
        return false;
    }

    // Set User-Agent header.
    if (!WinHttpAddRequestHeaders(hRequest, (L"User-Agent: " + userAgent).c_str(), -1, WINHTTP_ADDREQ_FLAG_REPLACE)) {
        std::cerr << "Failed to set User-Agent header\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        return false;
    }

    // Send request.
    if (!WinHttpSendRequest(hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0,
        0, 0)) {
        std::cerr << "Failed to send HTTP request\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        return false;
    }

    // Receive response.
    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        std::cerr << "Failed to receive response\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        return false;
    }

    // Read response data.
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPWSTR pszOutBuffer = NULL;
    bool success = false;

    do {
        // Check for available data.
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            std::cerr << "Error in WinHttpQueryDataAvailable.\n";
            break;
        }

        // Allocate space for the buffer.
        pszOutBuffer = new WCHAR[dwSize / sizeof(WCHAR) + 1];
        if (!pszOutBuffer) {
            std::cerr << "Out of memory\n";
            dwSize = 0;
            break;
        }

        // Read the data.
        ZeroMemory(pszOutBuffer, dwSize + sizeof(WCHAR));
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
            dwSize, &dwDownloaded)) {
            std::cerr << "Error in WinHttpReadData.\n";
            delete[] pszOutBuffer;
            break;
        }

        // Append data to response.
        response.append(pszOutBuffer, dwDownloaded / sizeof(WCHAR));

        // Free the memory allocated to the buffer.
        delete[] pszOutBuffer;

        success = true;
    } while (dwSize > 0);

    // Close request handle.
    WinHttpCloseHandle(hRequest);
    // Close connection handle.
    WinHttpCloseHandle(hConnect);

    return success;
}