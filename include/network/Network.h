#ifndef NETWORK_H
#define NETWORK_H

#define WM_H24RECV (WM_USER + 2)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <DirectXMath.h>
#include <WinSock2.h>

#include <string>

struct SendData
{
    DirectX::XMMATRIX transform;
};

class Network
{
public:
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
