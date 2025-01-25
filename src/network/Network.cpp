#include "network/Network.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WS2tcpip.h>

#include <iostream>
#include <print>
#include <utility>

void Network::InitWinsock()
{
    WSADATA wsa_data;

    int r = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (r != 0)
    {
        std::println("WSAStartup failed with error: {}", r);
        return;
    }
}

void Network::CreateSocket()
{
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET)
    {
        std::println("socket failed with error: {}", WSAGetLastError());
        WSACleanup();
        return;
    }

    m_partnerAddr.sin_family = AF_INET;
    m_partnerAddr.sin_port = htons(m_port);
    int r = inet_pton(AF_INET, m_partnerIpAddr.c_str(), &m_partnerAddr.sin_addr);
    if (r == 0)
    {
        std::println("inet_pton failed with error: {}", WSAGetLastError());
        WSACleanup();
        return;
    }

    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(m_port);
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    r = bind(m_sock, reinterpret_cast<sockaddr*>(&clientAddr), sizeof(clientAddr));
    if (r == SOCKET_ERROR)
    {
        std::println("bind failed with error: {}", WSAGetLastError());
        closesocket(m_sock);
        WSACleanup();
        return;
    }
}

void Network::Listen()
{
    std::cout << "Listening..." << std::endl;

    while (true)
    {
        char recvbuf[sizeof(SendData)];
        int r = recv(m_sock, recvbuf, sizeof(SendData), 0);
        if (r == SOCKET_ERROR)
        {
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
            closesocket(m_sock);
            WSACleanup();
            return;
        }

        SendData* data = new SendData;
        memcpy(data, recvbuf, sizeof(SendData));

        // std::cout << "Received data" << std::endl;

        SendMessage(m_hwnd, WM_H24RECV, 0, reinterpret_cast<LPARAM>(data));
    }
}

HRESULT Network::Send(const SendData& data)
{
    char buf[sizeof(SendData)];
    memcpy(buf, &data, sizeof(SendData));

    int r = sendto(
        m_sock,
        buf,
        sizeof(buf),
        0,
        reinterpret_cast<sockaddr*>(&m_partnerAddr),
        sizeof(m_partnerAddr)
    );
    if (r == SOCKET_ERROR)
    {
        // std::cout << "sendto failed with error: " << WSAGetLastError()
        //           << std::endl;
        closesocket(m_sock);
        WSACleanup();
        return E_FAIL;
    }

    return S_OK;
}
