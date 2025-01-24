#ifndef UICOMPONENT_H
#define UICOMPONENT_H
#include <AquaEngine.h>

#include <memory>

class UIComponent
{
public:
    UIComponent() : m_data()
    {
    }

    void Init(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& texture_segment,
        int texture_offset,
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& matrix_segment,
        int matrix_offset,
        const std::string& texturePath,
        AquaEngine::Command& command
    );
    void Render(AquaEngine::Command& command);

    void SetPosition(float x, float y);
    void SetColor(float r, float g, float b);
    void SetScale(float x, float y);

    void SetAlpha(float alpha)
    {
        m_data.alpha = alpha;
        m_dataBuffer.GetMappedBuffer()->alpha = m_data.alpha;
    }

    void RotateAndSinScale(float angle)
    {
        m_angle += angle;
        m_afterScale = 1.0f + std::sin(m_angle) * 0.2f;
    }

    void Rotate(float angle)
    {
        m_angle += angle;
        UpdateMatrix();
    }

private:
    struct Data
    {
        DirectX::XMMATRIX matrix;
        DirectX::XMFLOAT3 color;
        float alpha;
    };

    std::unique_ptr<AquaEngine::RectangleTexture> m_rectangle;
    AquaEngine::GPUBuffer<Data> m_dataBuffer;
    AquaEngine::ConstantBufferView m_dataCBV;

    Data m_data;
    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;
    float m_afterScale = 1.0f;
    float m_angle = 0.0f;

    float m_x = 0.0f;
    float m_y = 0.0f;

    void UpdateMatrix();
};

#endif  // UICOMPONENT_H
