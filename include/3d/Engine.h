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

    void TranslatePartner(const DirectX::XMMATRIX &transform) const
    {
        m_gameView->TransformPartner(transform);
    }

    void MoveModel(float dx, float dy, float dz) const
    {
        m_gameView->MoveModel(dx, dy, dz);
    }

    void SetStartStatus(StartStatus status)
    {
        m_startStatus = status;
    }

    void SetIsPlayer1(bool isPlayer1)
    {
        m_gameView->SetIsPlayer1(isPlayer1);
    }

    void Accel()
    {
        m_gameView->Accel();
    }

    void Decel()
    {
        m_gameView->Decel();
    }

    void RotRight()
    {
        m_gameView->RotRight();
    }

    void RotLeft()
    {
        m_gameView->RotLeft();
    }

    void RotUp()
    {
        m_gameView->RotUp();
    }

    void RotDown()
    {
        m_gameView->RotDown();
    }

    [[nodiscard]] StartStatus GetStartStatus() const
    {
        return m_startStatus;
    }

    [[nodiscard]] DirectX::XMMATRIX GetMatrix() const
    {
        return m_gameView->GetMatrix();
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
