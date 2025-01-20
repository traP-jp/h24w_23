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
        HWND hWndParent
    ) override;

    void InitNetwork(const std::string& ipaddr, bool isServer)
    {
        m_network = std::make_unique<Network>(ipaddr, m_hwnd);
        m_network->CreateSocket();

        m_engine->SetIsPlayer1(isServer);
    }

    void Listen() const
    {
        m_network->Listen();
    }

private:
    std::unique_ptr<Engine> m_engine;
    std::unique_ptr<Network> m_network;

    LPCSTR ClassName() const override
    {
        return "MainWindow";
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void Send();
};

#endif  // MAINWINDOW_H
