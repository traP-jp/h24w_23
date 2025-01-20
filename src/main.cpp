#include <AquaEngine.h>
#include <windows.h>

#include <iostream>

#include "view/IPDialog.h"
#include "view/MainWindow.h"
#include "view/resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
)
{
    MessageBox(nullptr, "Hello, Windows Desktop!", "Aqua Engine", MB_OK);

    DialogBox(
        hInstance,
        MAKEINTRESOURCE(IDD_DIALOG1),
        nullptr,
        reinterpret_cast<DLGPROC>(IPDialog::DlgProc)
    );

    std::cout << IPDialog::GetIPAddr() << std::endl;

    MainWindow window;
    HRESULT hr = window.Create(
        "MainWindow",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        0,
        1920,
        1200,
        nullptr
    );
    if (FAILED(hr))
    {
        return -1;
    }

    ShowWindow(window.Window(), nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
