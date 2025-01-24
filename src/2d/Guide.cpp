#include "2d/Guide.h"

void Guide::Init(
    AquaEngine::Command& command,
    AquaEngine::DescriptorHeapSegmentManager& manager
)
{
    m_rectangle = std::make_unique<AquaEngine::RectangleTexture>(
        DirectX::XMFLOAT3(-0.7f, 0.8f, 0.0f),
        DirectX::XMFLOAT3(0.7f, 0.8f, 0.0f),
        DirectX::XMFLOAT3(-0.7f, -0.8f, 0.0f),
        DirectX::XMFLOAT3(0.7f, -0.8f, 0.0f),
        "resources/models/UI1.png",
        command
    );
    m_rectangle->Create();
    auto tex_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_rectangle->CreateShaderResourceView(std::move(tex_range), manager);
}

void Guide::Render(AquaEngine::Command& command)
{
    m_rectangle->Render(command);
}
