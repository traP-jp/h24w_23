#ifndef SIDEUI_H
#define SIDEUI_H

#include <AquaEngine.h>

class SideUI
{
public:
    void Init(AquaEngine::Command &command);
    void CreatePipelineState();
    void Render(AquaEngine::Command &command);
    void UseRootSignature(AquaEngine::Command &command) const;

    void Move(float dx, float dy, float dz) const
    {
        m_model->Move(dx, dy, dz);
    }

    void Scale(float x, float y, float z) const
    {
        m_model->Scale(x, y, z);
    }

private:
    std::unique_ptr<AquaEngine::FBXModel> m_model;

    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    std::unique_ptr<AquaEngine::Buffer> m_alpha1Texture;
    AquaEngine::ShaderResourceView m_alpha1SRV;
    std::unique_ptr<AquaEngine::Buffer> m_alpha2Texture;
    AquaEngine::ShaderResourceView m_alpha2SRV;

    void ImportModel(AquaEngine::Command &command);
};

#endif  // SIDEUI_H
