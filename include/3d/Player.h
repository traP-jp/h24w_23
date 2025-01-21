#ifndef PLAYER_H
#define PLAYER_H

#include <AquaEngine.h>

#include <memory>

class Player
{
public:
    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command) const;
    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index
    void SetTextureSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index
    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index

    void Move(float dx, float dy, float dz) const;
    void Scale(float x, float y, float z) const;
    void Rot(float x, float y, float z) const;
    void Timer() const;

    void Accel()
    {
        m_velocity += acceleration;
    }

    void Decel()
    {
        m_velocity -= deceleration;
    }

    void SetMatrix(const DirectX::XMMATRIX& matrix) const;

    DirectX::XMMATRIX GetMatrix() const
    {
        return m_body->GetMatrix();
    }

    std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputElementDescs() const
    {
        return m_body->GetInputElementDescs();
    }

private:
    std::unique_ptr<AquaEngine::FBXModel> m_arm;
    std::unique_ptr<AquaEngine::FBXModel> m_body;
    std::unique_ptr<AquaEngine::FBXModel> m_buster;
    std::unique_ptr<AquaEngine::FBXModel> m_eye;
    std::unique_ptr<AquaEngine::FBXModel> m_gun;
    std::unique_ptr<AquaEngine::FBXModel> m_head;
    std::unique_ptr<AquaEngine::FBXModel> m_ring;
    std::unique_ptr<AquaEngine::FBXModel> m_thuraster;

    DirectX::XMVECTOR m_direction = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    float m_velocity = 0.0f;

    static constexpr float acceleration = 0.025f;
    static constexpr float deceleration = -0.025f;
    static constexpr float max_velocity = 10.0f;

    void ImportModel(AquaEngine::Command& command);
};

#endif  // PLAYER_H
