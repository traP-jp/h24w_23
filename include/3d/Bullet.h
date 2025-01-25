#ifndef BULLET_H
#define BULLET_H

#include <AquaEngine.h>
#include <Effekseer.h>

#include <memory>

// TODO: Bullet Rotate

class Bullet
{
public:
    void Init(AquaEngine::Command &command);
    void CreateEffect(const Effekseer::ManagerRef &manager);
    void Render(AquaEngine::Command &command) const;
    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;
    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;
    void SetShaderResourceView(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;

    void Shoot(
        DirectX::XMMATRIX transform,
        DirectX::XMMATRIX coordinate,
        DirectX::XMVECTOR direction
    )
    {
        m_direction = direction;
        m_model->SetTransformMatrix(transform);
        m_model->SetCoordinateMatrix(coordinate);
        m_model->SetScale(DEFAULT_SCALE, DEFAULT_SCALE, DEFAULT_SCALE);

        DirectX::XMVECTOR position = m_model->GetPos();
        // std::cout << "bullet position: " << DirectX::XMVectorGetX(position) << ", "
        //           << DirectX::XMVectorGetY(position) << ", " << DirectX::XMVectorGetZ(position)
        //           << std::endl;
        m_isActive = true;
    }

    void Frame() const
    {
        if (!m_isActive)
        {
            return;
        }
        m_model->Move(
            DirectX::XMVectorGetX(m_direction) * VELOCITY,
            DirectX::XMVectorGetY(m_direction) * VELOCITY,
            DirectX::XMVectorGetZ(m_direction) * VELOCITY
        );
        //
        // std::cout << "bullet position: "
        //           << DirectX::XMVectorGetX(m_model->GetPos()) << ", "
        //           << DirectX::XMVectorGetY(m_model->GetPos()) << ", "
        //           << DirectX::XMVectorGetZ(m_model->GetPos()) << std::endl;
    }

    void Move(float dx, float dy, float dz) const
    {
        m_model->Move(dx, dy, dz);
    }

    bool IsHit(DirectX::XMVECTOR position, float radius, const Effekseer::ManagerRef &manager)
    {
        if (!m_isActive)
        {
            return false;
        }
        DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(m_model->GetPos(), position);
        float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(distance));

        if (length < radius)
        {
            PlayEffect(manager);
            m_isActive = false;

            std::cout << "bullet position: "
                      << DirectX::XMVectorGetX(m_model->GetPos())
                             + DirectX::XMVectorGetX(m_direction) * VELOCITY * 2
                      << ", "
                      << DirectX::XMVectorGetY(m_model->GetPos())
                             + DirectX::XMVectorGetY(m_direction) * VELOCITY * 2
                      << ", "
                      << DirectX::XMVectorGetZ(m_model->GetPos())
                             + DirectX::XMVectorGetZ(m_direction) * VELOCITY * 2
                      << std::endl;
        }

        return length < radius;
    }

    void PlayEffect(const Effekseer::ManagerRef &manager)
    {
        if (!m_isActive || m_isEffectActive)
        {
            return;
        }
        DirectX::XMVECTOR position = m_model->GetPos();
        m_handle = manager->Play(
            m_effect,
            DirectX::XMVectorGetX(position) + DirectX::XMVectorGetX(m_direction) * VELOCITY * 2,
            DirectX::XMVectorGetY(position) + DirectX::XMVectorGetY(m_direction) * VELOCITY * 2,
            DirectX::XMVectorGetZ(position) + DirectX::XMVectorGetZ(m_direction) * VELOCITY * 2
        );
        manager->SetLocation(
            m_handle,
            DirectX::XMVectorGetX(position) + DirectX::XMVectorGetX(m_direction) * VELOCITY * 2,
            DirectX::XMVectorGetY(position) + DirectX::XMVectorGetY(m_direction) * VELOCITY * 2,
            DirectX::XMVectorGetZ(position) + DirectX::XMVectorGetZ(m_direction) * VELOCITY * 2
        );
        manager->SetScale(m_handle, EFFECT_SCALE, EFFECT_SCALE, EFFECT_SCALE);

        m_isEffectActive = true;

        // std::cout << "play effect" << std::endl;
    }

private:
    static constexpr float VELOCITY = 1.0f;
    static constexpr float DEFAULT_SCALE = 0.5f;
    static constexpr float EFFECT_SCALE = 1.0f;

    DirectX::XMVECTOR m_direction{};

    std::unique_ptr<AquaEngine::FBXModel> m_model;
    bool m_isActive = false;
    bool m_isEffectActive = false;

    Effekseer::EffectRef m_effect = nullptr;
    Effekseer::Handle m_handle = 0;

    void ImportModel(AquaEngine::Command &command);
};

#endif  // BULLET_H
