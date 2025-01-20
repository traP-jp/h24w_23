#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#define TIMER_MODEL1 1
#define TIMER_MODEL2 2

#include <AquaEngine.h>

#include <memory>

class GameView
{
public:
    GameView(HWND hwnd, RECT rc);
    ~GameView() = default;

    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);
    void Timer(int id) const;
    void StartAnimation();

private:
    HWND m_hwnd;

    std::unique_ptr<AquaEngine::FBXModel> m_model1;
    std::unique_ptr<AquaEngine::FBXModel> m_model2;

    AquaEngine::PipelineState m_pipelineState;
    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::Camera m_camera;
};

#endif  // GAMEVIEW_H
