#include "3d/Asteroid.h"

void Asteroid::Init(AquaEngine::Command &command, bool is1)
{
    ImportModel(command, is1);
}

void Asteroid::Render(AquaEngine::Command &command)
{
    m_model->Render(command);
}

void Asteroid::SetMatrixSegments(const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment, int index) const
{
    m_model->CreateMatrixBuffer(segment, index);
}

void Asteroid::SetTextureSegments(const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment, int index) const
{
    m_model->SetTexture(segment, index);
}

void Asteroid::SetMaterialSegments(const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment, int index) const
{
    m_model->CreateMaterialBufferView(segment, index);
}

void Asteroid::ImportModel(AquaEngine::Command &command, bool is1)
{
    if (is1)
    {
        m_model = std::make_unique<AquaEngine::FBXModel>(
            "resources/models/asteroid_patern1.fbx",
            "resources/models/asteroid_patern1_diff_tex.png",
            command
        );
    } else
    {
        m_model = std::make_unique<AquaEngine::FBXModel>(
            "resources/models/asteroid_patern2.fbx",
            "resources/models/asteroid_patern1_diff_tex.png",
            command
        );
    }
    m_model->Create();
}
