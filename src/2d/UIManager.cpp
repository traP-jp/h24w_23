#include "2d/UIManager.h"

void UIManager::Init(AquaEngine::Command& command)
{
    auto& tex_manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "ui",
            1,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

    m_guide.Init(command, tex_manager);

    m_rootSignature.AddStaticSampler(
        AquaEngine::RootSignature::DefaultStaticSampler()
    );
    m_rootSignature.SetDescriptorHeapSegmentManager(&tex_manager);
    HRESULT hr = m_rootSignature.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create root signature" << std::endl;
        exit(-1);
    }

    auto input = AquaEngine::RectangleTexture::GetInputElementDescs();

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"shaders/ui.hlsl", "vs", "vs_5_0");
    ps.Load(L"shaders/ui.hlsl", "ps", "ps_5_0");
    m_pipelineState.SetRootSignature(&m_rootSignature);
    m_pipelineState.SetVertexShader(&vs);
    m_pipelineState.SetPixelShader(&ps);
    m_pipelineState.SetInputLayout(input.data(), input.size());
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

void UIManager::Render(AquaEngine::Command& command)
{
    m_pipelineState.SetToCommand(command);
    m_rootSignature.SetToCommand(command);

    m_guide.Render(command);
}
