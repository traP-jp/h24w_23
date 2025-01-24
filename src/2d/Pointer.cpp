#include "2d/Pointer.h"

void Pointer::Init(AquaEngine::Command& command)
{
    auto& manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "pointer",
            2,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );
    m_rectangle = std::make_unique<AquaEngine::RectangleTexture>(
        DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
        "resources/models/UI3.png",
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

    auto inputElement = m_rectangle->GetInputElementDescs();

    m_rootSignature.AddStaticSampler(
        AquaEngine::RootSignature::DefaultStaticSampler()
    );
    m_rootSignature.SetDescriptorHeapSegmentManager(&manager);
    HRESULT hr = m_rootSignature.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create root signature" << std::endl;
        exit(-1);
    }

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"shaders/ui3.hlsl", "vs", "vs_5_0");
    ps.Load(L"shaders/ui3.hlsl", "ps", "ps_5_0");

    m_pipelineState.SetRootSignature(&m_rootSignature);
    m_pipelineState.SetVertexShader(&vs);
    m_pipelineState.SetPixelShader(&ps);
    m_pipelineState.SetInputLayout(inputElement.data(), inputElement.size());
    D3D12_BLEND_DESC blendDesc
        = {.AlphaToCoverageEnable = FALSE,
           .IndependentBlendEnable = FALSE,
           .RenderTarget
           = {{.BlendEnable = TRUE,
               .SrcBlend = D3D12_BLEND_SRC_ALPHA,
               .DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
               .BlendOp = D3D12_BLEND_OP_ADD,
               .SrcBlendAlpha = D3D12_BLEND_ONE,
               .DestBlendAlpha = D3D12_BLEND_ZERO,
               .BlendOpAlpha = D3D12_BLEND_OP_ADD,
               .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL}}};
    m_pipelineState.SetBlendState(blendDesc);
    hr = m_pipelineState.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create pipeline state" << std::endl;
        exit(-1);
    }
}

void Pointer::Render(AquaEngine::Command& command)
{
    m_pipelineState.SetToCommand(command);
    m_rootSignature.SetToCommand(command);
    m_rectangle->Render(command);
}

void Pointer::SetPosition(float x, float y)
{
}
