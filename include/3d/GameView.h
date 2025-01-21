#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#define TIMER_MODEL1 1
#define TIMER_MODEL2 2
#define TIMER_FRAME  10

#include <AquaEngine.h>

#include <memory>

#include "Player.h"

class GameView
{
public:
    GameView(HWND hwnd, RECT rc);
    ~GameView() = default;

    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);
    void Timer(int id) const;
    void StartAnimation();
    void Start();

    void SetIsPlayer1(bool is_player1)
    {
        m_isPlayer1 = is_player1;
    }

    void TransformPartner(const DirectX::XMMATRIX& transform) const
    {
        (m_isPlayer1 ? m_playerModel2 : m_playerModel1).SetMatrix(transform);
    }

    void MoveModel(float dx, float dy, float dz) const
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Move(dx, dy, dz);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void Accel()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Accel();
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void Decel()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Decel();
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotRight()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotRight();
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotLeft()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotLeft();
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    DirectX::XMMATRIX GetMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetMatrix();
    }

private:
    HWND m_hwnd;
    RECT m_rc;

    Player m_playerModel1;
    Player m_playerModel2;
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

    float angle = 0.0f;
};

#endif  // GAMEVIEW_H
