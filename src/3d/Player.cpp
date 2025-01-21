#include "3d/Player.h"

void Player::Init(AquaEngine::Command& command)
{
    ImportModel(command);
}

void Player::Render(AquaEngine::Command& command)
{
    m_arm->Render(command);
    m_body->Render(command);
    m_buster->Render(command);
    m_eye->Render(command);
    m_gun->Render(command);
    m_head->Render(command);
    m_ring->Render(command);
    m_thuraster->Render(command);
}

void Player::ImportModel(AquaEngine::Command& command)
{
    m_arm = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/arm.fbx",
        "resources/models/arm_diff_tex.png",
        command
    );
    m_arm->Create();

    m_body = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/body.fbx",
        "resources/models/body_diff_tex.png",
        command
    );
    m_body->Create();

    m_buster = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/buster.fbx",
        "resources/models/buster_diff_tex.png",
        command
    );
    m_buster->Create();

    m_eye = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/eye.fbx"
    );
    m_eye->Create();

    m_gun = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/gun.fbx",
        "resources/models/gun_diff_tex.png",
        command
    );
    m_gun->Create();

    m_head = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/head.fbx",
        "resources/models/head_diff_tex.png",
        command
    );
    m_head->Create();

    m_ring = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/ring.fbx",
        "resources/models/ring_diff_tex.png",
        command
    );
    m_ring->Create();

    m_thuraster = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/thuraster.fbx",
        "resources/models/thuraster_diff_tex.png",
        command
    );
    m_thuraster->Create();
}

void Player::SetMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
    const int index
) const
{
    m_arm->CreateMatrixBuffer(segment, 8 * index);
    m_body->CreateMatrixBuffer(segment, 8 * index + 1);
    m_buster->CreateMatrixBuffer(segment, 8 * index + 2);
    m_eye->CreateMatrixBuffer(segment, 8 * index + 3);
    m_gun->CreateMatrixBuffer(segment, 8 * index + 4);
    m_head->CreateMatrixBuffer(segment, 8 * index + 5);
    m_ring->CreateMatrixBuffer(segment, 8 * index + 6);
    m_thuraster->CreateMatrixBuffer(segment, 8 * index + 7);
}

void Player::SetTextureSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
    const int index
) const
{
    m_arm->SetTexture(segment, 7 * index);
    m_body->SetTexture(segment, 7 * index + 1);
    m_buster->SetTexture(segment, 7 * index + 2);
    m_gun->SetTexture(segment, 7 * index + 3);
    m_head->SetTexture(segment, 7 * index + 4);
    m_ring->SetTexture(segment, 7 * index + 5);
    m_thuraster->SetTexture(segment, 7 * index + 6);
}

void Player::SetMaterialSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
    const int index
) const
{
    m_arm->CreateMaterialBufferView(segment, 8 * index);
    m_body->CreateMaterialBufferView(segment, 8 * index + 1);
    m_buster->CreateMaterialBufferView(segment, 8 * index + 2);
    m_eye->CreateMaterialBufferView(segment, 8 * index + 3);
    m_gun->CreateMaterialBufferView(segment, 8 * index + 4);
    m_head->CreateMaterialBufferView(segment, 8 * index + 5);
    m_ring->CreateMaterialBufferView(segment, 8 * index + 6);
    m_thuraster->CreateMaterialBufferView(segment, 8 * index + 7);
}
