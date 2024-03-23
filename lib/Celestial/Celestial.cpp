#include "Celestial.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")


std::string http_get_request(const std::string& url) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "";
    }

    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return "";
    }

    std::istringstream iss(url);
    std::string host, path;
    std::getline(std::getline(iss, host, '/'), path);

    struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host.c_str(), "http", &hints, &result) != 0) {
        std::cerr << "getaddrinfo failed." << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return "";
    }
    

    ptr = result;

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = ((sockaddr_in*)ptr->ai_addr)->sin_addr.s_addr;
    clientService.sin_port = htons(80);

    if (connect(ConnectSocket, (SOCKADDR*) & clientService, sizeof(clientService)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect." << std::endl;
        freeaddrinfo(result);
        closesocket(ConnectSocket);
        WSACleanup();
        return "";
    }

    std::ostringstream request_stream;
    request_stream << "GET /" << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "\r\n";
    std::string request = request_stream.str();

    if (send(ConnectSocket, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed." << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return "";
    }

    std::string response;
    char buffer[1024];
    int bytesReceived;
    do {
        bytesReceived = recv(ConnectSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0)
            response.append(buffer, bytesReceived);
        else if (bytesReceived == 0)
            std::cerr << "Connection closed." << std::endl;
        else
            std::cerr << "Recv failed." << std::endl;
    } while (bytesReceived > 0);

    closesocket(ConnectSocket);
    WSACleanup();

    return response;
}
