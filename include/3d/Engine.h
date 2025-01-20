#ifndef ENGINE_H
#define ENGINE_H

#include <AquaEngine.h>
#include <windows.h>

#include <memory>

#include "2d/D2DEngine.h"

class Engine
{
public:
    Engine(HWND hwnd, RECT wr);
    ~Engine();

    void Init();
    void Render() const;

private:
    std::unique_ptr<AquaEngine::Command> m_command;
    std::unique_ptr<AquaEngine::Display> m_display;

    std::unique_ptr<D2DEngine> m_d2dEngine;

    HWND m_hwnd;
    RECT m_wr;
};

#endif  // ENGINE_H
