#ifndef ENGINE_H
#define ENGINE_H

#define TIMER_TITLE 3

#include <AquaEngine.h>
#include <windows.h>

#include <memory>

#include "2d/D2DEngine.h"
#include "GameView.h"

class Engine
{
public:
    enum class StartStatus
    {
        TITLE,
        STARTING,
        RUNNING
    };

    Engine(HWND hwnd, RECT wr);
    ~Engine();

    void Init();
    void Render();
    void Timer(int id);

    void SetStartStatus(StartStatus status)
    {
        m_startStatus = status;
    }

    [[nodiscard]] StartStatus GetStartStatus() const
    {
        return m_startStatus;
    }

private:
    struct Weight
    {
        float gameWeight;
    };

    std::unique_ptr<AquaEngine::Command> m_command;
    std::unique_ptr<AquaEngine::Display> m_display;

    AquaEngine::RenderTarget m_gameRenderTarget;
    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;
    AquaEngine::GPUBuffer<Weight> m_weightBuffer;
    AquaEngine::ConstantBufferView m_weightCBV;

    float m_gameWeight = 0.0f;

    std::unique_ptr<D2DEngine> m_d2dEngine;
    std::unique_ptr<GameView> m_gameView;

    HWND m_hwnd;
    RECT m_wr;

    StartStatus m_startStatus = StartStatus::TITLE;
    float m_elapsedTime = 0.0f;

    void InitRenderTargets();
};

#endif  // ENGINE_H
