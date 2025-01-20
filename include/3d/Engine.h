#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>

class Engine
{
public:
    Engine(HWND hwnd, RECT wr);

    void Init();
    void Render();
};

#endif  // ENGINE_H
