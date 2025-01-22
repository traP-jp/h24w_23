#include "3d/Camera.h"

void Camera::Init()
{
    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera = std::make_shared<AquaEngine::Camera>(m_rc);
    m_camera->Init(
        m_isPlayer1 ? PLAYER1_DEFAULT_CAMERA : PLAYER2_DEFAULT_CAMERA,
        m_isPlayer1 ? PLAYER1_DEFAULT_POSITION : PLAYER2_DEFAULT_POSITION,
        {0.0f, 2.0f, 1.0f}
    );
    m_camera->AddManager("main_game", std::move(camera_range));
}

void Camera::Render(AquaEngine::Command& command) const
{
    m_camera->Render(command, "main_game");
}

void Camera::Accel(DirectX::XMVECTOR delta)
{
    m_camera->Move(
                DirectX::XMVectorGetX(delta),
                DirectX::XMVectorGetY(delta),
                DirectX::XMVectorGetZ(delta)
            );
}

void Camera::Decel(DirectX::XMVECTOR delta)
{
    m_camera->Move(
            DirectX::XMVectorGetX(delta),
            DirectX::XMVectorGetY(delta),
            DirectX::XMVectorGetZ(delta)
        );
}

void Camera::RotRight(const DirectX::XMMATRIX& transform)
{
    m_camera->Rot(transform);
}

void Camera::RotLeft(const DirectX::XMMATRIX& transform)
{
    m_camera->Rot(transform);
}

void Camera::RotUp(const DirectX::XMMATRIX& transform)
{
    m_camera->Rot(transform);
}

void Camera::RotDown(const DirectX::XMMATRIX& transform)
{
    m_camera->Rot(transform);
}

void Camera::Move(DirectX::XMVECTOR dr)
{
    m_camera->Move(
        DirectX::XMVectorGetX(dr),
        DirectX::XMVectorGetY(dr),
        DirectX::XMVectorGetZ(dr)
    );
}
