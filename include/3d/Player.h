#ifndef PLAYER_H
#define PLAYER_H

#include <AquaEngine.h>

#include <memory>

using DirectX::operator*;

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
    void RotX(float angle) const;
    void RotY(float angle) const;
    void RotZ(float angle) const;
    void Timer() const;
    void Frame() const;

    DirectX::XMVECTOR Accel()
    {
        m_velocity += ACCELERATION;

        if (m_velocity > MAX_VELOCITY)
        {
            m_velocity = MAX_VELOCITY;
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        return m_direction * -ACCELERATION * 2;
    }

    DirectX::XMVECTOR Decel()
    {
        m_velocity += DECELERATION;

        if (m_velocity < 0.0f)
        {
            m_velocity = 0.0f;
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        return m_direction * -DECELERATION * 2;
    }

    void RotRight()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(0.1f)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(0.1f);
        }
    }

    void RotLeft()
    {
        m_direction = XMVector3Transform(
            XMVector3Transform(
                DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::XMMatrixRotationY(-0.1f)
            ),
            m_models[0]->GetTransformMatrix()
        );
        m_direction = DirectX::XMVector3Normalize(m_direction);

        for (int i = 0; i < m_models.size(); ++i)
        {
            m_models[i]->RotY(-0.1f);
        }
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
            m_models[i]->RotX(-0.1f);
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
            m_models[i]->RotX(0.1f);
        }
    }

    void SetMatrix(const DirectX::XMMATRIX& matrix) const;

    float GetVelocity() const
    {
        return m_velocity;
    }

    DirectX::XMMATRIX GetTransformMatrix() const
    {
        return m_models[0]->GetTransformMatrix();
    }

    DirectX::XMMATRIX GetMatrix() const
    {
        return m_models[0]->GetMatrix();
    }

    std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputElementDescs() const
    {
        return m_models[0]->GetInputElementDescs();
    }

    UINT GetFrameCount() const
    {
        return m_models[2]->GetFrameCount();
    }

    DirectX::XMVECTOR GetDrForCamera() const
    {
        return m_direction * m_velocity;
    }

    DirectX::XMVECTOR GetDirection() const
    {
        return m_direction;
    }

private:
    std::array<std::unique_ptr<AquaEngine::FBXModel>, 8> m_models;

    DirectX::XMVECTOR m_direction
        = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    float m_velocity = 0.0f;
    float m_yAngle = 0.0f;
    float m_xAngle = 0.0f;

    static constexpr float ACCELERATION = 0.05f;
    static constexpr float DECELERATION = -0.15f;
    static constexpr float MAX_VELOCITY = 10.0f;

    void ImportModel(AquaEngine::Command& command);
};

#endif  // PLAYER_H
