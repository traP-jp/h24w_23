#include <AquaEngine.h>
#include <windows.h>

#include <iostream>
#include <thread>

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
    std::string ip_addr;
    bool is_player1 = false;
    std::thread t(
        []()
        {
            DialogBox(
                GetModuleHandle(nullptr),
                MAKEINTRESOURCE(IDD_DIALOG1),
                nullptr,
                reinterpret_cast<DLGPROC>(IPDialog::DlgProc)
            );
        }
    );

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

    t.join();

    ip_addr = IPDialog::GetIPAddr();
    is_player1 = IPDialog::IsPlayer1();

    ShowWindow(window.Window(), nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}