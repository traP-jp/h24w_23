#ifndef ENGINE_H
#define ENGINE_H

#include <AquaEngine.h>
#include <windows.h>

#include <memory>

#include "2d/D2DEngine.h"
#include "GameView.h"

class Engine
{
public:
    Engine(HWND hwnd, RECT wr);
    ~Engine();

    void Init();
    void Render();

private:
    std::unique_ptr<AquaEngine::Command> m_command;
    std::unique_ptr<AquaEngine::Display> m_display;

    std::unique_ptr<D2DEngine> m_d2dEngine;
    std::unique_ptr<GameView> m_gameView;

    HWND m_hwnd;
    RECT m_wr;

    bool m_isStarted;
};

#endif  // ENGINE_H
