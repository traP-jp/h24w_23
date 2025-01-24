#include "2d/UIComponent.h"

void UIComponent::Init(
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& texture_segment,
    int texture_offset,
    const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& matrix_segment,
    int matrix_offset,
    const std::string& texturePath,
    AquaEngine::Command& command
)
{
    m_rectangle = std::make_unique<AquaEngine::RectangleTexture>(
        DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
        texturePath,
        command
    );
    m_rectangle->Create();

    m_data.color = {1.0f, 1.0f, 1.0f};
    m_data.matrix = DirectX::XMMatrixIdentity();
    m_data.alpha = 1.0f;

    m_dataBuffer.Create(BUFFER_DEFAULT(
        AquaEngine::AlignmentSize(sizeof(Data), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)
    ));
    m_dataBuffer.GetMappedBuffer()->color = m_data.color;
    m_dataBuffer.GetMappedBuffer()->alpha = m_data.alpha;
    m_dataBuffer.GetMappedBuffer()->matrix = m_data.matrix;

    m_dataCBV.SetDescriptorHeapSegment(matrix_segment, matrix_offset);
    m_dataCBV.Create(m_dataBuffer.GetBuffer());

    m_rectangle->CreateShaderResourceView(texture_segment, texture_offset);
}

void UIComponent::Render(AquaEngine::Command& command)
{
    m_dataCBV.SetGraphicsRootDescriptorTable(&command);
    m_rectangle->Render(command);
}

void UIComponent::SetPosition(float x, float y)
{
    m_data.matrix = DirectX::XMMatrixTranslation(x, y, 0.0f)
                    * DirectX::XMMatrixScaling(m_scaleX, m_scaleY, 1.0f);
    m_dataBuffer.GetMappedBuffer()->matrix = m_data.matrix;
}

void UIComponent::SetColor(float r, float g, float b)
{
    m_data.color = {r, g, b};
    m_dataBuffer.GetMappedBuffer()->color = m_data.color;
}

void UIComponent::SetScale(float x, float y)
{
    m_scaleX = x;
    m_scaleY = y;
    m_data.matrix *= DirectX::XMMatrixScaling(x, y, 1.0f);
    m_dataBuffer.GetMappedBuffer()->matrix = m_data.matrix;
}
