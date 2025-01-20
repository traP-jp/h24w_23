#ifndef ENGINE_H
#define ENGINE_H

#include <AquaEngine.h>
#include <windows.h>

#include <memory>

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

    HWND m_hwnd;
    RECT m_wr;
};

#endif  // ENGINE_H
