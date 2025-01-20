#include "view/MainWindow.h"

HRESULT MainWindow::Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent
)
{
    HRESULT hr = BaseWindow<MainWindow>::Create(
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

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}
