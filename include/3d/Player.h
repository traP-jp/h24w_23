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

    void RenderEye(AquaEngine::Command &command) const;

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
    void SetEyeMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;  // this is player index
    void SetEyeMaterialSegments(
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
        CreateBusterEffect(manager);
        CreateGunEffect(manager);
    }

    void Move(float dx, float dy, float dz) const;

    void Scale(float x, float y, float z) const;

    void RotX(float angle) const;

    void RotY(float angle) const;

    void RotZ(float angle) const;

    void Timer() const;

    void Frame();

    void BulletFrame() const;

    void Shoot(DirectX::XMVECTOR direction, float velocity);

    bool GetIsAccel() const
    {
        return m_isaccel;
    }

    int GetAccelFrame() const
    {
        return m_accelFrame;
    }

    DirectX::XMVECTOR Accel()
    {
        // m_velocity += ACCELERATION;
        //
        // if (m_velocity > MAX_VELOCITY)
        // {
        //     m_velocity = MAX_VELOCITY;
        //     return {0.0f, 0.0f, 0.0f, 0.0f};
        // }
        // // return m_direction * -ACCELERATION * 0.2;

        if (m_isaccel)
        {
            return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        }

        m_isaccel = true;
        m_isaccel2 = true;
        m_accelFrame = 0;
        return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    }

    DirectX::XMVECTOR Decel()
    {
        m_velocity += DECELERATION;

        if (m_velocity < 0.0f)
        {
            m_velocity = 0.0f;
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        // return m_direction * -DECELERATION * 0.2;
        return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
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

        m_up = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
                DirectX::XMMatrixRotationY(ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_up = DirectX::XMVector3Normalize(m_up);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(ROTATION);
        }

        m_yAngle += ROTATION;

        m_manager->SetRotation(m_busterHandle, m_xAngle - DirectX::XM_PIDIV2, m_yAngle, 0);
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

        m_up = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
                DirectX::XMMatrixRotationY(-ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_up = DirectX::XMVector3Normalize(m_up);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(-ROTATION);
        }

        m_yAngle -= ROTATION;

        m_manager->SetRotation(m_busterHandle, m_xAngle - DirectX::XM_PIDIV2, m_yAngle, 0);
    }

    void RotUp()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationX(-ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        m_up = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
                DirectX::XMMatrixRotationX(-ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_up = DirectX::XMVector3Normalize(m_up);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotX(-ROTATION);
        }

        m_xAngle -= ROTATION;

        m_manager->SetRotation(m_busterHandle, m_xAngle - DirectX::XM_PIDIV2, m_yAngle, 0);
    }

    void RotDown()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationX(ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        m_up = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
                DirectX::XMMatrixRotationX(ROTATION)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_up = DirectX::XMVector3Normalize(m_up);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotX(ROTATION);
        }

        m_xAngle += ROTATION;

        m_manager->SetRotation(m_busterHandle, m_xAngle - DirectX::XM_PIDIV2, m_yAngle, 0);
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

    void SetVelocity(float velocity)
    {
        m_velocity = velocity;
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

    [[nodiscard]] DirectX::XMVECTOR GetUp() const
    {
        return m_up;
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
    static constexpr float ACCELERATION = 0.2f;
    static constexpr float DECELERATION = -0.6f;
    static constexpr float MAX_VELOCITY = 50.0f;
    static constexpr float ROTATION = 0.1f;
    static constexpr float RADIUS = DEFAULT_SCALE * 1201.28f;

    static constexpr float BUSTER_EFFECT_SCALE = 0.4f;
    static constexpr float GUN_EFFECT_SCALE = 0.14f;

    std::array<std::unique_ptr<AquaEngine::FBXModel>, 8> m_models;
    std::array<Bullet, BULLET_COUNT> m_bullets;
    int m_bulletIndex = 0;

    DirectX::XMVECTOR m_direction = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMVECTOR m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    float m_velocity = 0.0f;
    float m_yAngle = 0.0f;
    float m_xAngle = 0.0f;

    Effekseer::EffectRef m_busterEffect = nullptr;
    Effekseer::Handle m_busterHandle = 0;

    Effekseer::EffectRef m_gunEffect = nullptr;
    Effekseer::Handle m_gunHandle = 0;

    Effekseer::ManagerRef m_manager;

    int m_accelFrame = 0;
    bool m_isaccel = false;
    bool m_isaccel2 = false;

    static constexpr float ACCEL_FAST = 1.2f;
    static constexpr float ACCEL_SLOW = 0.2f;

    // camera mo henshu
    static constexpr int ACCEL_FIRST_FRAME = 3;
    static constexpr int ACCEL_SECOND_FRAME = 15;
    static constexpr int ACCEL_THIRD_FRAME = 21;

    void ImportModel(AquaEngine::Command &command);

    void CreateBusterEffect(const Effekseer::ManagerRef &manager);
    void CreateGunEffect(const Effekseer::ManagerRef &manager);
};

#endif  // PLAYER_H
