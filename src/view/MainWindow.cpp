#include "view/MainWindow.h"

HRESULT MainWindow::Create(
    LPCSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent
)
{
    HRESULT hr = BaseWindow::Create(
        lpWindowName,
        dwStyle,
        dwExStyle,
        x,
        y,
        nWidth,
        nHeight,
        hWndParent
    );
    if (FAILED(hr))
    {
        return hr;
    }

    m_engine = std::make_unique<Engine>(m_hwnd, wr);
    m_engine->Init();

    return S_OK;
}

void MainWindow::Send()
{
    SendData data = {m_engine->GetMatrix()};
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
            PAINTSTRUCT ps;
            BeginPaint(m_hwnd, &ps);

            m_engine->Render();

            EndPaint(m_hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (m_engine->GetStartStatus() == Engine::StartStatus::TITLE)
            {
                SetTimer(m_hwnd, TIMER_TITLE, 25, nullptr);
                m_engine->SetStartStatus(Engine::StartStatus::STARTING);
            }

            if (m_engine->GetStartStatus() == Engine::StartStatus::RUNNING)
            {
                case VK_LEFT:
                    m_engine->MoveModel(-0.1f, 0.0f, 0.0f);
                    Send();
                    break;

                case VK_RIGHT:
                    m_engine->MoveModel(0.1f, 0.0f, 0.0f);
                    Send();
                    break;

                case VK_UP:
                    m_engine->MoveModel(0.0f, 0.0f, 0.1f);
                    Send();
                    break;

                case VK_DOWN:
                    m_engine->MoveModel(0.0f, 0.0f, -0.1f);
                    Send();
                    break;
            }
            return 0;
        }

        case WM_TIMER:
            m_engine->Timer(wParam);
            return 0;

        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}
