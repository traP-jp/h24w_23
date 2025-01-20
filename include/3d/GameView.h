#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#define TIMER_MODEL1 1
#define TIMER_MODEL2 2

#include <AquaEngine.h>

#include <memory>

class GameView
{
public:
    GameView(HWND hwnd, RECT rc, bool isPlayer1);
    ~GameView() = default;

    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);
    void Timer(int id) const;
    void StartAnimation();

    void Transform(const DirectX::XMMATRIX& transform) const
    {
        if (m_isPlayer1)
        {
            m_model1->SetMatrix(transform);
        }
        else
        {
            m_model2->SetMatrix(transform);
        }
    }

private:
    HWND m_hwnd;
    RECT m_rc;

    std::unique_ptr<AquaEngine::FBXModel> m_model1;
    std::unique_ptr<AquaEngine::FBXModel> m_model2;
    std::unique_ptr<AquaEngine::SkyBox> m_skyBox;
    std::shared_ptr<AquaEngine::Camera> m_camera;

    AquaEngine::PipelineState m_pipelineState;
    AquaEngine::RootSignature m_rootSignature;

    bool m_isPlayer1;

    void CreateModels(
        AquaEngine::Command& command,
        AquaEngine::DescriptorHeapSegmentManager& manager
    );
    void CreateSkyBox(AquaEngine::Command& command);
};

#endif  // GAMEVIEW_H
