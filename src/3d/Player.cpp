#include "3d/Player.h"

#include <iostream>

void Player::Init(AquaEngine::Command &command)
{
    ImportModel(command);
}

void Player::Render(AquaEngine::Command &command) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->Render(command);
    }
}

void Player::ImportModel(AquaEngine::Command &command)
{
    m_models[0] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/arm.fbx",
        "resources/models/arm_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[0] = std::make_unique<AquaEngine::FBXModel>("resources/models/arm.fbx");
    m_models[0]->Create();

    m_models[1] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/body.fbx",
        "resources/models/body_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[1] = std::make_unique<AquaEngine::FBXModel>("resources/models/body.fbx");
    m_models[1]->Create();

    m_models[2] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/buster.fbx",
        "resources/models/buster_diff_tex_lrud.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[2] = std::make_unique<AquaEngine::FBXModel>("resources/models/buster.fbx");
    m_models[2]->Create();

    m_models[3]
        = std::make_unique<AquaEngine::FBXModel>("resources/models/eye.fbx");
    m_models[3]->Create();

    m_models[4] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/gun.fbx",
        "resources/models/gun_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[4] = std::make_unique<AquaEngine::FBXModel>("resources/models/gun.fbx");
    m_models[4]->Create();

    m_models[5] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/head.fbx",
        "resources/models/head_diff_tex2.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[5] = std::make_unique<AquaEngine::FBXModel>("resources/models/head.fbx");
    m_models[5]->Create();

    m_models[6] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/ring.fbx",
        "resources/models/ring_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[6] = std::make_unique<AquaEngine::FBXModel>("resources/models/ring.fbx");
    m_models[6]->Create();

    m_models[7] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/thuraster.fbx",
        "resources/models/thuraster_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    //m_models[7] = std::make_unique<AquaEngine::FBXModel>("resources/models/thuraster.fbx");
    m_models[7]->Create();
}

void Player::SetMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    const int index
) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->CreateMatrixBuffer(segment, 8 * index + i);
    }
}

void Player::SetTextureSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    const int index
) const
{
    for (int i = 0; i < 3; ++i)
    {
        m_models[i]->SetTexture(segment, 7 * index + i);
    }

    for (int i = 4; i < m_models.size(); ++i)
    {
        m_models[i]->SetTexture(segment, 7 * index + i - 1);
    }
}

void Player::SetMaterialSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    const int index
) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->CreateMaterialBufferView(segment, 8 * index + i);
    }
}

void Player::Move(float dx, float dy, float dz) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->Move(dx, dy, dz);
    }
}

void Player::SetMatrix(const DirectX::XMMATRIX &matrix) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->SetMatrix(matrix);
    }
}

void Player::Timer() const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->Timer();
    }
}

void Player::Frame() const
{
    DirectX::XMVECTOR dr = m_direction * m_velocity;
    Move(dr.m128_f32[0], dr.m128_f32[1], dr.m128_f32[2]);
}

void Player::Scale(float x, float y, float z) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->Scale(x, y, z);
    }
}

void Player::RotX(float angle) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->RotX(angle);
    }
}

void Player::RotY(float angle) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->RotY(angle);
    }
}

void Player::RotZ(float angle) const
{
    for (int i = 0; i < m_models.size(); ++i)
    {
        m_models[i]->RotZ(angle);
    }
}
