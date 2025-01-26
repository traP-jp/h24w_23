#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#define TIMER_MODEL1        1
#define TIMER_MODEL2        2
#define TIMER_FRAME         10

#define H24ACTION_ACCEL     0x1
#define H24ACTION_DECEL     0x2
#define H24ACTION_ROT_RIGHT 0x4
#define H24ACTION_ROT_LEFT  0x8
#define H24ACTION_ROT_UP    0x10
#define H24ACTION_ROT_DOWN  0x20
#define H24ACTION_SHOOT     0x40

#include <AquaEngine.h>

#include <memory>

#include "2d/Result.h"
#include "2d/UIManager.h"
#include "Asteroid.h"
#include "Camera.h"
#include "EffectManager.h"
#include "Player.h"
#include "SideUI.h"

using DirectX::operator-;
using DirectX::operator+;

class GameView
{
public:
    GameView(HWND hwnd, RECT rc, bool is_player1);

    ~GameView() = default;

    void Init(AquaEngine::Command &command);

    void Render(AquaEngine::Command &command);

    void Timer(int id);

    void StartAnimation();

    void Start();

    void SetIsPlayer1(bool is_player1)
    {
        m_isPlayer1 = is_player1;
    }

    void TransformPartner(const DirectX::XMMATRIX &transform) const
    {
        (m_isPlayer1 ? m_playerModel2 : m_playerModel1).SetMatrix(transform);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
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
        DirectX::XMVECTOR delta = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Decel();
        m_camera->Decel(delta);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotRight()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotRight();
        DirectX::XMVECTOR direction
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
        DirectX::XMVECTOR up = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetUp();
        m_camera->StartFrame(direction, up);
        m_camera->Rot((m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix());
        m_camera->EndFrame(direction, up);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotLeft()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotLeft();
        DirectX::XMVECTOR direction
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
        DirectX::XMVECTOR up = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetUp();
        m_camera->StartFrame(direction, up);
        m_camera->Rot((m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix());
        m_camera->EndFrame(direction, up);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotUp()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotUp();
        DirectX::XMVECTOR direction
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
        DirectX::XMVECTOR up = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetUp();
        m_camera->StartFrame(direction, up);
        m_camera->Rot((m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix());
        m_camera->EndFrame(direction, up);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void RotDown()
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).RotDown();
        DirectX::XMVECTOR direction
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
        DirectX::XMVECTOR up = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetUp();
        m_camera->StartFrame(direction, up);
        m_camera->Rot((m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix());
        m_camera->EndFrame(direction, up);
        InvalidateRect(m_hwnd, &m_rc, FALSE);
    }

    void Shoot()
    {
        DirectX::XMVECTOR r;
        if (m_isPlayer1)
        {
            r = m_playerModel2.GetPos() - m_playerModel1.GetPos();
        }
        else
        {
            r = m_playerModel1.GetPos() - m_playerModel2.GetPos();
        }

        r = DirectX::XMVector3Normalize(r);

        float velocity = DirectX::XMVector3Dot(
                             (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection(),
                             r
        )
                             .m128_f32[0];

        DirectX::XMVECTOR direction;
        if (m_isPlayer1)
        {
            direction = m_playerModel2.GetDirection() * m_playerModel2.GetVelocity()
                        + (velocity + Bullet::VELOCITY_CONST - m_playerModel2.GetVelocity()) * r;
        }
        else
        {
            direction = m_playerModel1.GetDirection() * m_playerModel1.GetVelocity()
                        + (velocity + Bullet::VELOCITY_CONST - m_playerModel1.GetVelocity()) * r;
        }

        direction = DirectX::XMVector3Normalize(direction);

        if (velocity
                / DirectX::XMVector3Length(
                      (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection()
                )
                      .m128_f32[0]
            < 0.6f)
        {
            direction = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
            velocity = Bullet::VELOCITY_CONST * 4.0f;
        }

        (m_isPlayer1 ? m_playerModel1 : m_playerModel2)
            .Shoot(direction, velocity + Bullet::VELOCITY_CONST);
        m_bullets--;
    }

    void PartnerShoot()
    {
        DirectX::XMVECTOR r;
        if (m_isPlayer1)
        {
            r = m_playerModel1.GetPos() - m_playerModel2.GetPos();
        }
        else
        {
            r = m_playerModel2.GetPos() - m_playerModel1.GetPos();
        }

        r = DirectX::XMVector3Normalize(r);

        float velocity = DirectX::XMVector3Dot(
                             (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetDirection(),
                             r
        )
                             .m128_f32[0];

        DirectX::XMVECTOR direction;
        if (m_isPlayer1)
        {
            direction = m_playerModel1.GetDirection() * m_playerModel1.GetVelocity()
                        + (velocity + Bullet::VELOCITY_CONST - m_playerModel1.GetVelocity()) * r;
        }
        else
        {
            direction = m_playerModel2.GetDirection() * m_playerModel2.GetVelocity()
                        + (velocity + Bullet::VELOCITY_CONST - m_playerModel2.GetVelocity()) * r;
        }

        direction = DirectX::XMVector3Normalize(direction);

        if (velocity
                / DirectX::XMVector3Length(
                      (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetDirection()
                )
                      .m128_f32[0]
            < 0.6f)
        {
            float delta
                = (0.6f
                   - velocity
                         / DirectX::XMVector3Length(
                               (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetDirection()
                         )
                               .m128_f32[0])
                  * 2;
            direction = DirectX::XMVector3Normalize(
                (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetDirection() * delta
                + direction * 0.4f
            );
        }

        (m_isPlayer1 ? m_playerModel2 : m_playerModel1)
            .Shoot(direction, velocity + Bullet::VELOCITY_CONST);
    }

    void SetPartner(
        const DirectX::XMMATRIX &transform,
        const DirectX::XMMATRIX &coordinate,
        DirectX::XMVECTOR direction
    )
    {
        (m_isPlayer1 ? m_playerModel2 : m_playerModel1).SetTransformMatrix(transform);
        (m_isPlayer1 ? m_playerModel2 : m_playerModel1).SetCoordinateMatrix(coordinate);
        (m_isPlayer1 ? m_playerModel2 : m_playerModel1).SetDirection(direction);

        DirectX::XMVECTOR pos = (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetPos();
        // std::cout << "partner position: " << DirectX::XMVectorGetX(pos) << ", "
        //           << DirectX::XMVectorGetY(pos) << ", " << DirectX::XMVectorGetZ(pos) <<
        //           std::endl;
    }

    DirectX::XMMATRIX GetPartnerTransformMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetTransformMatrix();
    }

    DirectX::XMMATRIX GetPartnerCoordinateMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetCoordinateMatrix();
    }

    DirectX::XMVECTOR GetPartnerDirection() const
    {
        return (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetDirection();
    }

    DirectX::XMMATRIX GetTransformMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetTransformMatrix();
    }

    DirectX::XMMATRIX GetCoordinateMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetCoordinateMatrix();
    }

    DirectX::XMVECTOR GetDirection() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
    }

    DirectX::XMMATRIX GetMatrix() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetMatrix();
    }

    float GetPlayerVelocity() const
    {
        return (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetVelocity();
    }

    int GetBullets() const
    {
        return m_bullets;
    }

private:
    enum class GameStatus
    {
        GAMING,
        WIN,
        LOSE
    };

    HWND m_hwnd;
    RECT m_rc;

    Player m_playerModel1;
    Player m_playerModel2;
    std::unique_ptr<AquaEngine::SkyBox> m_skyBox;
    std::unique_ptr<Camera> m_camera;
    std::array<Asteroid, 100> m_asteroids;
    EffectManager m_effectManager;

    AquaEngine::PipelineState m_pipelineState;
    AquaEngine::RootSignature m_rootSignature;

    AquaEngine::PipelineState m_bulletPipelineState;
    AquaEngine::RootSignature m_bulletRootSignature;
    std::unique_ptr<AquaEngine::Buffer> m_bulletAlphaTexture;
    AquaEngine::ShaderResourceView m_bulletAlphaSRV;

    AquaEngine::PipelineState m_eyePipelineState;
    AquaEngine::RootSignature m_eyeRootSignature;

    AquaEngine::DirectionLight m_directionLight;

    UIManager m_uiManager;
    SideUI m_sideUI;
    Result m_result;

    GameStatus m_gameStatus = GameStatus::GAMING;

    bool m_isPlayer1;
    int m_bullets = Player::BULLET_COUNT;

    void CreateModels(
        AquaEngine::Command &command,
        AquaEngine::DescriptorHeapSegmentManager &manager
    );

    void CreateSkyBox(AquaEngine::Command &command);

    void CreateUI(AquaEngine::Command &command);

    float angle = 0.0f;

    // Camera.hも一緒に変える
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_POSITION = {1.0f, 0.0f, 0.0f};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_POSITION = {-1.0f, 0.0f, 0.0f};

    static constexpr float DEFAULT_CAMERA_Z_DISTANCE = -4.2f;
    static constexpr float CAMERA_ASPECT = 1.2f;
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_CAMERA
        = {1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE *CAMERA_ASPECT};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_CAMERA
        = {-1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE *CAMERA_ASPECT};
};

#endif  // GAMEVIEW_H
