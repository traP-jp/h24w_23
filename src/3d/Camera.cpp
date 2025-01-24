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

    m_delta = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void Camera::InitBullet()
{
    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera->AddManager("bullet", std::move(camera_range));
}

void Camera::InitSideUI()
{
    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        2,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera->AddManager("side_ui", std::move(camera_range));
}

void Camera::RenderSideUI(AquaEngine::Command &command) const
{
    m_camera->Render(command, "side_ui");
}

void Camera::Render(AquaEngine::Command &command) const
{
    m_camera->Render(command, "main_game");
}

void Camera::RenderBullet(AquaEngine::Command &command) const
{
    m_camera->Render(command, "bullet");
}

void Camera::Accel(DirectX::XMVECTOR delta)
{
    m_camera->Move(
        DirectX::XMVectorGetX(delta),
        DirectX::XMVectorGetY(delta),
        DirectX::XMVectorGetZ(delta)
    );

    m_delta = DirectX::XMVectorAdd(m_delta, delta);
}

void Camera::Decel(DirectX::XMVECTOR delta)
{
    m_camera->Move(
        DirectX::XMVectorGetX(delta),
        DirectX::XMVectorGetY(delta),
        DirectX::XMVectorGetZ(delta)
    );

    m_delta = DirectX::XMVectorAdd(m_delta, delta);
}

void Camera::RotRight(const DirectX::XMMATRIX &transform)
{
    m_camera->Move(
        -DirectX::XMVectorGetX(m_delta),
        -DirectX::XMVectorGetY(m_delta),
        -DirectX::XMVectorGetZ(m_delta)
    );
    m_camera->Rot(transform);
    m_camera->Move(
        DirectX::XMVectorGetX(m_delta),
        DirectX::XMVectorGetY(m_delta),
        DirectX::XMVectorGetZ(m_delta)
    );
}

void Camera::RotLeft(const DirectX::XMMATRIX &transform)
{
    m_camera->Move(
        -DirectX::XMVectorGetX(m_delta),
        -DirectX::XMVectorGetY(m_delta),
        -DirectX::XMVectorGetZ(m_delta)
    );
    m_camera->Rot(transform);
    m_camera->Move(
        DirectX::XMVectorGetX(m_delta),
        DirectX::XMVectorGetY(m_delta),
        DirectX::XMVectorGetZ(m_delta)
    );
}

void Camera::RotUp(const DirectX::XMMATRIX &transform)
{
    m_camera->Move(
        -DirectX::XMVectorGetX(m_delta),
        -DirectX::XMVectorGetY(m_delta),
        -DirectX::XMVectorGetZ(m_delta)
    );
    m_camera->Rot(transform);
    m_camera->Move(
        DirectX::XMVectorGetX(m_delta),
        DirectX::XMVectorGetY(m_delta),
        DirectX::XMVectorGetZ(m_delta)
    );
}

void Camera::RotDown(const DirectX::XMMATRIX &transform)
{
    m_camera->Move(
        -DirectX::XMVectorGetX(m_delta),
        -DirectX::XMVectorGetY(m_delta),
        -DirectX::XMVectorGetZ(m_delta)
    );
    m_camera->Rot(transform);
    m_camera->Move(
        DirectX::XMVectorGetX(m_delta),
        DirectX::XMVectorGetY(m_delta),
        DirectX::XMVectorGetZ(m_delta)
    );
}

void Camera::Move(DirectX::XMVECTOR dr)
{
    m_camera->Move(DirectX::XMVectorGetX(dr), DirectX::XMVectorGetY(dr), DirectX::XMVectorGetZ(dr));
}
