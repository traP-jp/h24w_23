#include <AquaEngine.h>
#include <windows.h>

#include <iostream>

#include "view/IPDialog.h"
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

    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, 800, 600};

    HWND hwnd = CreateWindowEx(
        0,
        "WindowClass",
        "Hello, Engine!",
        WS_OVERLAPPEDWINDOW,
        wr.left,
        wr.top,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    if (hwnd == nullptr)
    {
        return -1;
    }

    // d3d init
    AquaEngine::Factory::Init(true);
    AquaEngine::Device::GetAdaptors();
    AquaEngine::Device::Init(1);
    AquaEngine::GlobalDescriptorHeapManager::Init();
    AquaEngine::Command command;
    AquaEngine::Display display(hwnd, wr, command);
    display.SetBackgroundColor(1.0f, 1.0f, 0.0f, 1.0f);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        display.BeginRender();
        display.SetViewports();
        display.EndRender();

        command.Execute();

        display.Present();
    }

    AquaEngine::GlobalDescriptorHeapManager::Shutdown();
    AquaEngine::Device::Shutdown();
    AquaEngine::Factory::Shutdown();

    UnregisterClass("WindowClass", hInstance);

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
