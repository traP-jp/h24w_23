#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#define TIMER_MODEL1 1
#define TIMER_MODEL2 2
#define TIMER_FRAME  10

#include <AquaEngine.h>

#include <memory>

#include "Camera.h"
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
        DirectX::XMVECTOR delta
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Accel();
        m_camera->Accel(delta);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void Decel()
    {
        DirectX::XMVECTOR delta
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Decel();
        m_camera->Decel(delta);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotRight()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotRight();
        m_camera->RotRight(
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix()
        );
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotLeft()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotLeft();
        m_camera->RotLeft(
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix()
        );
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotUp()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotUp();
        m_camera->RotUp(
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix()
        );
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotDown()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotDown();
        m_camera->RotDown(
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix()
        );
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
    std::shared_ptr<Camera> m_camera;

    AquaEngine::PipelineState m_pipelineState;
    AquaEngine::RootSignature m_rootSignature;

    bool m_isPlayer1;

    void CreateModels(
        AquaEngine::Command& command,
        AquaEngine::DescriptorHeapSegmentManager& manager
    );
    void CreateSkyBox(AquaEngine::Command& command);

    float angle = 0.0f;

    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_POSITION
        = {1.0f, 0.0f, 0.0f};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_POSITION
        = {-1.0f, 0.0f, 0.0f};

    static constexpr float DEFAULT_CAMERA_Z_DISTANCE = -2.2f;
    static constexpr float CAMERA_ASPECT = 0.8f;
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_CAMERA
        = {1.0f,
           -DEFAULT_CAMERA_Z_DISTANCE,
           DEFAULT_CAMERA_Z_DISTANCE* CAMERA_ASPECT};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_CAMERA
        = {-1.0f,
           -DEFAULT_CAMERA_Z_DISTANCE,
           DEFAULT_CAMERA_Z_DISTANCE* CAMERA_ASPECT};

    static constexpr float DEFAULT_SCALE = 0.002f;
};

#endif  // GAMEVIEW_H
