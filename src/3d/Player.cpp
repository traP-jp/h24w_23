#include "3d/Player.h"

#include <iostream>

void Player::Init(AquaEngine::Command &command)
{
    ImportModel(command);

    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].Init(command);
    }
}

void Player::Render(AquaEngine::Command &command) const
{
    for (int i = 0; i < 3; ++i)
    {
        m_models[i]->Render(command);
    }

    for (int i = 4; i < m_models.size(); ++i)
    {
        m_models[i]->Render(command);
    }
}

void Player::RenderEye(AquaEngine::Command &command) const
{
    m_models[3]->Render(command);
}

void Player::RenderBullet(AquaEngine::Command &command) const
{
    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].Render(command);
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
    // m_models[0] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/arm.fbx");
    m_models[0]->Create();

    m_models[1] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/body.fbx",
        "resources/models/body_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[1] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/body.fbx");
    m_models[1]->Create();

    m_models[2] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/buster.fbx",
        "resources/models/buster_diff_tex_lrud.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[2] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/buster.fbx");
    m_models[2]->Create();

    m_models[3] = std::make_unique<AquaEngine::FBXModel>("resources/models/eye.fbx");
    m_models[3]->Create();

    m_models[4] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/gun.fbx",
        "resources/models/gun_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[4] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/gun.fbx");
    m_models[4]->Create();

    m_models[5] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/head.fbx",
        "resources/models/head_diff_tex2.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[5] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/head.fbx");
    m_models[5]->Create();

    m_models[6] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/ring.fbx",
        "resources/models/ring_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[6] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/ring.fbx");
    m_models[6]->Create();

    m_models[7] = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/thuraster.fbx",
        //"resources/models/thuraster_noanim.fbx",
        "resources/models/thuraster_diff_tex.png",
        //"resources/models/player_tex.png",
        command
    );
    // m_models[7] =
    // std::make_unique<AquaEngine::FBXModel>("resources/models/thuraster.fbx");
    m_models[7]->Create();
}

void Player::SetMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    const int index
) const
{
    for (int i = 0; i < 3; ++i)
    {
        m_models[i]->CreateMatrixBuffer(segment, 8 * index + i);
    }

    for (int i = 4; i < m_models.size(); ++i)
    {
        m_models[i]->CreateMatrixBuffer(segment, 8 * index + i - 1);
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
    for (int i = 0; i < 3; ++i)
    {
        m_models[i]->CreateMaterialBufferView(segment, 8 * index + i);
    }

    for (int i = 4; i < m_models.size(); ++i)
    {
        m_models[i]->CreateMaterialBufferView(segment, 8 * index + i - 1);
    }
}

void Player::SetEyeMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    int index
) const
{
    m_models[3]->CreateMatrixBuffer(segment, index);
}

void Player::SetEyeMaterialSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    int index
) const
{
    m_models[3]->CreateMaterialBufferView(segment, index);
}

void Player::SetBulletMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    int index
)
{
    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].SetMatrixSegments(segment, BULLET_COUNT * index + i);
    }
}

void Player::SetBulletMaterialSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    int index
)
{
    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].SetMaterialSegments(segment, BULLET_COUNT * index + i);
    }
}

void Player::SetBulletShaderResourceView(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
    int index
)
{
    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].SetShaderResourceView(segment, BULLET_COUNT * index + i);
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

    DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(m_direction);

    m_manager->SetLocation(
        m_busterHandle,
        DirectX::XMVectorGetX(m_models[2]->GetPos()) - DirectX::XMVectorGetX(direction) * 0.5f,
        DirectX::XMVectorGetY(m_models[2]->GetPos()) - DirectX::XMVectorGetY(direction) * 0.5f,
        DirectX::XMVectorGetZ(m_models[2]->GetPos()) - DirectX::XMVectorGetZ(direction) * 0.5f
    );
}

void Player::BulletFrame() const
{
    for (int i = 0; i < m_bullets.size(); ++i)
    {
        m_bullets[i].Frame();
    }
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

void Player::Shoot(DirectX::XMVECTOR direction, float velocity)
{
    if (m_bulletIndex >= m_bullets.size())
    {
        std::cout << "No more bullets" << std::endl;
        return;
    }

    m_bullets[m_bulletIndex].Shoot(
        m_models[4]->GetTransformMatrix(),
        m_models[4]->GetCoordinate(),
        direction,
        velocity
    );
    // std::cout << "model pos: " << m_models[4]->GetPos().m128_f32[0] << ", "
    //           << m_models[4]->GetPos().m128_f32[1] << ", " << m_models[4]->GetPos().m128_f32[2]
    //           << std::endl;
    m_bulletIndex++;

    DirectX::XMVECTOR position = m_models[4]->GetPos();
    DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(m_direction);
    DirectX::XMVECTOR left = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_up, dir));
    m_gunHandle = m_manager->Play(m_gunEffect, 0, 0, 0);
    m_manager->SetScale(m_gunHandle, GUN_EFFECT_SCALE, GUN_EFFECT_SCALE, GUN_EFFECT_SCALE);
    m_manager->SetLocation(
        m_gunHandle,
        DirectX::XMVectorGetX(position) + DirectX::XMVectorGetX(dir) * 2.4
            - DirectX::XMVectorGetX(left) * 0.5,
        DirectX::XMVectorGetY(position) + DirectX::XMVectorGetY(dir) * 2.4
            - DirectX::XMVectorGetY(left) * 0.5,
        DirectX::XMVectorGetZ(position) + DirectX::XMVectorGetZ(dir) * 2.4
            - DirectX::XMVectorGetZ(left) * 0.5
    );
}

void Player::CreateBusterEffect(const Effekseer::ManagerRef &manager)
{
    m_busterEffect = Effekseer::Effect::Create(manager, u"resources/effects/booster.efkefc");
    if (m_busterEffect == nullptr)
    {
        std::cout << "buster effect is null" << std::endl;
    }

    m_busterHandle = manager->Play(m_busterEffect, 0, 0, 0);
    manager
        ->SetScale(m_busterHandle, BUSTER_EFFECT_SCALE, BUSTER_EFFECT_SCALE, BUSTER_EFFECT_SCALE);

    DirectX::XMVECTOR position = m_models[2]->GetPos();
    manager->SetLocation(
        m_busterHandle,
        DirectX::XMVectorGetX(position),
        DirectX::XMVectorGetY(position),
        DirectX::XMVectorGetZ(position) - 0.5f
    );

    manager->SetRotation(m_busterHandle, -DirectX::XM_PIDIV2, 0, 0);

    m_manager = manager;
}

void Player::CreateGunEffect(const Effekseer::ManagerRef &manager)
{
    m_gunEffect = Effekseer::Effect::Create(manager, u"resources/effects/ef_lightning01.efkefc");
    if (m_gunEffect == nullptr)
    {
        std::cout << "gun effect is null" << std::endl;
    }
}
