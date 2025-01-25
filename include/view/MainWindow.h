#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include "3d/Engine.h"
#include "BaseWindow.h"
#include "network/Network.h"

class MainWindow : public BaseWindow
{
public:
    HRESULT Create(
        LPCSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle,
        int x,
        int y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        bool isPlayer1
    );

    void InitNetwork(const std::string& ipaddr)
    {
        m_network = std::make_unique<Network>(ipaddr, m_hwnd);
        m_network->CreateSocket();
    }

    void Listen() const
    {
        m_network->Listen();
    }

private:
    std::unique_ptr<Engine> m_engine;
    std::unique_ptr<Network> m_network;

    bool m_isAccel = false;
    bool m_isDecel = false;

    bool m_onW = false;
    bool m_onA = false;
    bool m_onS = false;
    bool m_onD = false;

    LPCSTR ClassName() const override
    {
        return "MainWindow";
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void Send(WPARAM wParam);
};

#endif  // MAINWINDOW_H
