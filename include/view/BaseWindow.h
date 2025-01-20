#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <Windows.h>

template<class DERIVED_TYPE>
class BaseWindow
{
public:
    BaseWindow() : m_hwnd(nullptr), wr()
    {
    }

    virtual HRESULT Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = nullptr
    )
    {
        wr = {0, 0, nWidth, nHeight};

        WNDCLASSEX wc
            = {.cbSize = sizeof(WNDCLASSEX),
               .style = CS_HREDRAW | CS_VREDRAW,
               .lpfnWndProc = DERIVED_TYPE::WindowProc,
               .cbClsExtra = 0,
               .cbWndExtra = 0,
               .hInstance = GetModuleHandle(nullptr),
               .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
               .hCursor = LoadCursor(nullptr, IDC_ARROW),
               .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
               .lpszMenuName = nullptr,
               .lpszClassName = ClassName(),
               .hIconSm = LoadIcon(nullptr, IDI_APPLICATION)};

        ATOM res = RegisterClassEx(&wc);
        if (res == 0)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        m_hwnd = CreateWindowEx(
            dwExStyle,
            ClassName(),
            lpWindowName,
            dwStyle,
            x,
            y,
            nWidth,
            nHeight,
            hWndParent,
            nullptr,
            GetModuleHandle(nullptr),
            this
        );
        if (m_hwnd == nullptr)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        return S_OK;
    }

    HWND Window() const
    {
        return m_hwnd;
    }

protected:
    static LRESULT CALLBACK
    WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT *pCreateStruct
                = reinterpret_cast<CREATESTRUCT *>(lParam);
            pThis = static_cast<DERIVED_TYPE *>(pCreateStruct->lpCreateParams);
            SetWindowLongPtr(
                hwnd,
                GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(pThis)
            );

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<DERIVED_TYPE *>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
        }

        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    virtual PCWSTR ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
    RECT wr;
};

#endif  // BASEWINDOW_H
