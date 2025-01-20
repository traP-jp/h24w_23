#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>

#include "3d/Engine.h"
#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    HRESULT Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle,
        int x,
        int y,
        int nWidth,
        int nHeight,
        HWND hWndParent
    ) override;

private:
    PCWSTR ClassName() const override
    {
        return L"MainWindow";
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    std::unique_ptr<Engine> m_engine;
};

#endif  // MAINWINDOW_H
