#include "3d/Camera.h"

using DirectX::operator-;
using DirectX::operator+;
using DirectX::operator*;

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

void Camera::InitEye()
{
    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera->AddManager("eye", std::move(camera_range));
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

void Camera::RenderEye(AquaEngine::Command &command) const
{
    m_camera->Render(command, "eye");
}

void Camera::StartFrame(DirectX::XMVECTOR direction, DirectX::XMVECTOR up) const
{
    DirectX::XMVECTOR cam
        = direction * DEFAULT_CAMERA_Z_DISTANCE * CAMERA_ASPECT + up * -DEFAULT_CAMERA_Z_DISTANCE;

    m_camera->Move(
        -DirectX::XMVectorGetX(m_delta),
        -DirectX::XMVectorGetY(m_delta),
        -DirectX::XMVectorGetZ(m_delta)
    );
    m_camera->Move(
        -DirectX::XMVectorGetX(cam),
        -DirectX::XMVectorGetY(cam),
        -DirectX::XMVectorGetZ(cam)
    );
}

void Camera::EndFrame(DirectX::XMVECTOR direction, DirectX::XMVECTOR up)
{
    DirectX::XMVECTOR camera
        = direction * DEFAULT_CAMERA_Z_DISTANCE * CAMERA_ASPECT + up * -DEFAULT_CAMERA_Z_DISTANCE;

    m_camera->Move(
        DirectX::XMVectorGetX(camera),
        DirectX::XMVectorGetY(camera),
        DirectX::XMVectorGetZ(camera)
    );
    m_delta = DirectX::XMVectorSet(
        DirectX::XMVectorGetX(direction) * m_deltaDistance,
        DirectX::XMVectorGetY(direction) * m_deltaDistance,
        DirectX::XMVectorGetZ(direction) * m_deltaDistance,
        0.0f
    );
    m_camera->Move(
        DirectX::XMVectorGetX(m_delta),
        DirectX::XMVectorGetY(m_delta),
        DirectX::XMVectorGetZ(m_delta)
    );
}

void Camera::Accel(int accelFrame)
{
    float distance = 0;
    if (accelFrame > 0 && accelFrame <= ACCEL_FIRST_FRAME)
    {
        distance = CAMERA_ACCEL_DISTANCE * static_cast<float>(accelFrame) / ACCEL_FIRST_FRAME;
        // std::cout << "first frame" << std::endl;
    }
    else if (accelFrame > ACCEL_FIRST_FRAME && accelFrame <= ACCEL_SECOND_FRAME)
    {
        distance = CAMERA_ACCEL_DISTANCE;
        // std::cout << "second frame" << std::endl;
    }
    else if (accelFrame > ACCEL_SECOND_FRAME && accelFrame <= ACCEL_THIRD_FRAME)
    {
        distance = CAMERA_ACCEL_DISTANCE * static_cast<float>(ACCEL_THIRD_FRAME - accelFrame)
                   / (ACCEL_THIRD_FRAME - ACCEL_SECOND_FRAME);
        // std::cout << "third frame" << std::endl;
    }

    m_deltaDistance = -distance;
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

void Camera::Rot(const DirectX::XMMATRIX &transform)
{
    m_camera->Rot(transform);
}

void Camera::Move(DirectX::XMVECTOR dr)
{
    m_camera->Move(DirectX::XMVectorGetX(dr), DirectX::XMVectorGetY(dr), DirectX::XMVectorGetZ(dr));
}
