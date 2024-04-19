#include <iostream>
#define SECURITY_WIN32
#include "Celestial.h"

int main() {
    HttpRequest request;

    std::wstring url = L"https://api.github.com/repos/sashaBazarov/Ponyscript/readme";
    std::wstring response;

    // Добавляем заголовок "Accept", чтобы указать, что мы ожидаем получить JSON-ответ.
    std::wstring acceptHeader = L"application/vnd.github.v3+json";

    if (request.sendGetRequestWithUserAgent(url, L"WinHTTP Example Program", response)) {
        std::wcout << L"Response:\n" << response << std::endl;
    } else {
        std::cerr << "Failed to send GET request\n";
    }

    return 0;
}