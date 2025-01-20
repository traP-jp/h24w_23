#ifndef NETWORK_H
#define NETWORK_H

#define WM_H24SEND (WM_USER + 1)
#define WM_H24RECV (WM_USER + 2)

#include <DirectXMath.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

class Network
{
public:
    struct SendData
    {
        DirectX::XMMATRIX transform;
    };

    Network(std::string partnerIpAddr, HWND hwnd)
        : m_partnerIpAddr(std::move(partnerIpAddr))
        , m_sock(INVALID_SOCKET)
        , m_partnerAddr()
        , m_hwnd(hwnd)
    {
    }

    void Listen();
    void CreateSocket();
    HRESULT Send(const SendData &data);

    static void InitWinsock();

private:
    std::string m_partnerIpAddr;
    const uint16_t m_port = 27018;
    SOCKET m_sock;
    sockaddr_in m_partnerAddr;

    HWND m_hwnd;
};

#endif  // NETWORK_H
