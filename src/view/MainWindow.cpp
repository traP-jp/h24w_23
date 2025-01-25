#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "view/MainWindow.h"

#include <iostream>
#include <ostream>

HRESULT MainWindow::Create(
    LPCSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    bool isPlayer1
)
{
    HRESULT hr
        = BaseWindow::Create(lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent);
    if (FAILED(hr))
    {
        return hr;
    }

    m_engine = std::make_unique<Engine>(m_hwnd, wr, isPlayer1);
    m_engine->Init();

    return S_OK;
}

void MainWindow::Send()
{
    SendData data = {m_engine->GetMatrix()};
    std::cout << "Sending data" << std::endl;
    m_network->Send(data);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            std::cout << "WM_PAINT" << std::endl;
            PAINTSTRUCT ps;
            BeginPaint(m_hwnd, &ps);

            m_engine->Render();

            EndPaint(m_hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
        {
            std::cout << "WM_KEYDOWN" << std::endl;
            if (m_engine->GetStartStatus() == Engine::StartStatus::TITLE)
            {
                SetTimer(m_hwnd, TIMER_TITLE, 25, nullptr);
                m_engine->SetStartStatus(Engine::StartStatus::STARTING);
            }

            if (m_engine->GetStartStatus() == Engine::StartStatus::RUNNING)
            {
                switch (wParam)
                {
                    case VK_SHIFT:
                        m_isAccel = true;
                        break;

                    case VK_SPACE:
                        m_isDecel = true;
                        break;

                    case 'W':
                        m_onW = true;
                        break;

                    case 'A':
                        m_onA = true;
                        break;

                    case 'S':
                        m_onS = true;
                        break;

                    case 'D':
                        m_onD = true;
                        break;
                }
            }
            return 0;
        }

        case WM_KEYUP:
        {
            std::cout << "WM_KEYUP" << std::endl;
            switch (wParam)
            {
                case VK_SHIFT:
                    m_isAccel = false;
                    break;

                case VK_SPACE:
                    m_isDecel = false;
                    break;

                case 'W':
                    m_onW = false;
                    break;

                case 'A':
                    m_onA = false;
                    break;

                case 'S':
                    m_onS = false;
                    break;

                case 'D':
                    m_onD = false;
                    break;
            }
        }

        case WM_TIMER:
        {
            std::cout << "WM_TIMER" << std::endl;
            if (wParam == TIMER_FRAME)
            {
                if (m_isDecel)
                {
                    m_engine->Decel();
                }
                else if (m_isAccel)
                {
                    m_engine->Accel();
                }

                if (m_onW)
                {
                    m_engine->RotUp();
                }

                if (m_onA)
                {
                    m_engine->RotLeft();
                }

                if (m_onS)
                {
                    m_engine->RotDown();
                }

                if (m_onD)
                {
                    m_engine->RotRight();
                }
            }
            m_engine->Timer(wParam);

#ifndef DEBUG
            Send();
#endif
            return 0;
        }

        case WM_H24RECV:
            std::cout << "WM_H24RECV" << std::endl;
            SendData data = *reinterpret_cast<SendData *>(lParam);
            m_engine->TranslatePartner(data.transform);
            return 0;

        case WM_LBUTTONDOWN:
            std::cout << "shoot" << std::endl;
            m_engine->Shoot();
            return 0;

        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}
