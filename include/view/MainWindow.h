#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>

#include "3d/Engine.h"
#include "BaseWindow.h"

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

private:
    LPCSTR ClassName() const override
    {
        return "MainWindow";
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    std::unique_ptr<Engine> m_engine;
};

#endif  // MAINWINDOW_H
