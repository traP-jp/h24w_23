#ifndef BULLET_H
#define BULLET_H

#include <AquaEngine.h>

#include <memory>

class Bullet
{
public:
    void Init(AquaEngine::Command &command);
    void Render(AquaEngine::Command &command) const;
    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;
    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;

    void Shoot(DirectX::XMVECTOR default_position)
    {
        m_model->SetPos(
            DirectX::XMVectorGetX(default_position),
            DirectX::XMVectorGetY(default_position),
            DirectX::XMVectorGetZ(default_position)
        );
        m_isActive = true;
    }

    void Frame(DirectX::XMVECTOR direction) const
    {
        if (!m_isActive)
        {
            return;
        }
        m_model->Move(
            DirectX::XMVectorGetX(direction) * VELOCITY,
            DirectX::XMVectorGetY(direction) * VELOCITY,
            DirectX::XMVectorGetZ(direction) * VELOCITY
        );
    }

    void Move(float dx, float dy, float dz) const
    {
        m_model->Move(dx, dy, dz);
    }

private:
    static constexpr float VELOCITY = 100.0f;
    static constexpr float DEFAULT_SCALE = 0.2f;

    std::unique_ptr<AquaEngine::FBXModel> m_model;
    bool m_isActive = false;

    void ImportModel(AquaEngine::Command &command);
};

#endif  // BULLET_H
