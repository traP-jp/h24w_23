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
    std::thread t(
        []()
        {
            DialogBox(
                GetModuleHandle(nullptr),
                MAKEINTRESOURCE(IDD_DIALOG1),
                nullptr,
                reinterpret_cast<DLGPROC>(IPDialog::DlgProc)
            );

            std::cout << IPDialog::GetIPAddr() << std::endl;
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

    ShowWindow(window.Window(), nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}