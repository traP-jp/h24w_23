#include "3d/Bullet.h"

void Bullet::Init(AquaEngine::Command& command)
{
    ImportModel(command);
}

void Bullet::Render(AquaEngine::Command& command) const
{
    if (!m_isActive)
    {
        return;
    }
    m_model->Render(command);
}

void Bullet::SetMatrixSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
    int index
) const
{
    m_model->CreateMatrixBuffer(segment, index);
}

void Bullet::SetMaterialSegments(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
    int index
) const
{
    m_model->CreateMaterialBufferView(segment, index);
}

void Bullet::ImportModel(AquaEngine::Command& command)
{
    m_model
        = std::make_unique<AquaEngine::FBXModel>("resources/models/cube.fbx");
    m_model->Create();
}
