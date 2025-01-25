#ifndef PLAYER_H
#define PLAYER_H

#include <AquaEngine.h>

#include <memory>

#include "Bullet.h"

using DirectX::operator*;

class Player
{
public:
    void Init(AquaEngine::Command &command);

    void Render(AquaEngine::Command &command) const;

    void RenderBullet(AquaEngine::Command &command) const;

    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;  // this is player index
    void SetTextureSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;  // this is player index
    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;  // this is player index

    void SetBulletMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index  // this is bullet index
    );
    void SetBulletMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index  // this is bullet index
    );
    void SetBulletShaderResourceView(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index  // this is bullet index
    );

    void CreateEffect(const Effekseer::ManagerRef &manager)
    {
        for (int i = 0; i < m_bullets.size(); ++i)
        {
            m_bullets[i].CreateEffect(manager);
        }
    }

    void Move(float dx, float dy, float dz) const;

    void Scale(float x, float y, float z) const;

    void RotX(float angle) const;

    void RotY(float angle) const;

    void RotZ(float angle) const;

    void Timer() const;

    void Frame() const;

    void Shoot();

    DirectX::XMVECTOR Accel()
    {
        m_velocity += ACCELERATION;

        if (m_velocity > MAX_VELOCITY)
        {
            m_velocity = MAX_VELOCITY;
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        return m_direction * -ACCELERATION * 0.2;
    }

    DirectX::XMVECTOR Decel()
    {
        m_velocity += DECELERATION;

        if (m_velocity < 0.0f)
        {
            m_velocity = 0.0f;
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        return m_direction * -DECELERATION * 0.2;
    }

    void RotRight()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(ROTATION);
        }

        std::cout << "TransformMatrix Rot: \n"
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[3] << std::endl;
    }

    void RotLeft()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(-ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(-ROTATION);
        }

        std::cout << "TransformMatrix Rot: \n"
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[3] << std::endl;
    }

    void RotUp()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(-m_yAngle)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotX(-ROTATION);
        }
    }

    void RotDown()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(-m_yAngle)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotX(ROTATION);
        }
    }

    void SetMatrix(const DirectX::XMMATRIX &matrix) const;

    void SetTransformMatrix(const DirectX::XMMATRIX &matrix) const
    {
        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->SetTransformMatrix(matrix);
        }
    }

    void SetCoordinateMatrix(const DirectX::XMMATRIX &matrix) const
    {
        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->SetCoordinateMatrix(matrix);
        }
    }

    void SetDirection(DirectX::XMVECTOR direction)
    {
        m_direction = direction;
    }

    bool IsHit(DirectX::XMVECTOR position, const Effekseer::ManagerRef &manager)
    {
        for (int i = 0; i < m_bullets.size(); ++i)
        {
            if (m_bullets[i].IsHit(position, RADIUS, manager))
            {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] float GetVelocity() const
    {
        return m_velocity;
    }

    [[nodiscard]] DirectX::XMVECTOR GetPos()
    {
        return m_models[0]->GetPos();
    }

    [[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const
    {
        std::cout << "When Get TransformMatrix: \n"
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[0].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[1].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[2].m128_f32[3] << "\n"
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[0] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[1] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[2] << " "
                  << m_models[0]->GetTransformMatrix().r[3].m128_f32[3] << std::endl;
        return m_models[0]->GetTransformMatrix();
    }

    [[nodiscard]] DirectX::XMMATRIX GetMatrix() const
    {
        return m_models[0]->GetMatrix();
    }

    [[nodiscard]] DirectX::XMMATRIX GetMappedMatrix() const
    {
        return m_models[0]->GetMappedMatrix();
    }

    [[nodiscard]] DirectX::XMMATRIX GetCoordinateMatrix() const
    {
        return m_models[0]->GetCoordinate();
    }

    [[nodiscard]] std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputElementDescs() const
    {
        return m_models[0]->GetInputElementDescs();
    }

    [[nodiscard]] UINT GetFrameCount() const
    {
        return m_models[2]->GetFrameCount();
    }

    [[nodiscard]] DirectX::XMVECTOR GetDrForCamera() const
    {
        return m_direction * m_velocity;
    }

    [[nodiscard]] DirectX::XMVECTOR GetDirection() const
    {
        return m_direction;
    }

    void PlayThurasterAction() const
    {
        m_models[7]->PlayAnimation(
            "THURASTER_bone|action3",
            AquaEngine::FBXModel::AnimationMode::LOOP
        );
    }

    void PlayBusterAction() const
    {
        m_models[2]->PlayAnimation(
            "buster_bone|action3",
            AquaEngine::FBXModel::AnimationMode::LOOP
        );
    }

    static constexpr int BULLET_COUNT = 10;
    static constexpr float DEFAULT_SCALE = 0.002f;

private:
    static constexpr float ACCELERATION = 0.5f;
    static constexpr float DECELERATION = -1.5f;
    static constexpr float MAX_VELOCITY = 100.0f;
    static constexpr float ROTATION = 0.1f;
    static constexpr float RADIUS = DEFAULT_SCALE * 1201.28f;

    std::array<std::unique_ptr<AquaEngine::FBXModel>, 8> m_models;
    std::array<Bullet, BULLET_COUNT> m_bullets;
    int m_bulletIndex = 0;

    DirectX::XMVECTOR m_direction = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    float m_velocity = 0.0f;
    float m_yAngle = 0.0f;
    float m_xAngle = 0.0f;

    void ImportModel(AquaEngine::Command &command);
};

#endif  // PLAYER_H
