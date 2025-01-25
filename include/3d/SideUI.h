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

    // Player::DEFAULT_SCALEとの比
    void Scale(float x, float y, float z)
    {
        m_scaleX = x;
        m_scaleY = y;
        m_scaleZ = z;
    }

    void Frame()
    {
        m_scroll += 0.01f;
        m_scrollBuffer.GetMappedBuffer()->scroll = m_scroll;
    }

    void SetTransformMatrix(const DirectX::XMMATRIX &player_transform)
    {
        m_model->SetMatrix(
            DirectX::XMMatrixScaling(m_scaleX, m_scaleY, m_scaleZ) * player_transform
        );
    }

private:
    struct Scroll
    {
        float scroll;
    };

    std::unique_ptr<AquaEngine::FBXModel> m_model;

    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    std::unique_ptr<AquaEngine::Buffer> m_alpha1Texture;
    AquaEngine::ShaderResourceView m_alpha1SRV;
    std::unique_ptr<AquaEngine::Buffer> m_alpha2Texture;
    AquaEngine::ShaderResourceView m_alpha2SRV;

    AquaEngine::GPUBuffer<Scroll> m_scrollBuffer;
    AquaEngine::ConstantBufferView m_scrollCBV;

    float m_scroll = 0.0f;
    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;
    float m_scaleZ = 1.0f;

    void ImportModel(AquaEngine::Command &command);
};

#endif  // SIDEUI_H
