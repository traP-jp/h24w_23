#include "2d/UIManager.h"

void UIManager::Init(AquaEngine::Command& command)
{
    auto& manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "ui",
            20,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );
    auto texture_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(3)
    );
    auto texture_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    texture_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(texture_range),
        1
    );

    auto matrix_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(3)
    );
    auto matrix_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    matrix_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(matrix_range),
        1
    );

    m_guide = std::make_unique<UIComponent>();
    m_guide->Init(
        texture_segment,
        0,
        matrix_segment,
        0,
        "resources/models/UI1.png",
        command
    );
    m_pointer = std::make_unique<UIComponent>();
    m_pointer->Init(
        texture_segment,
        1,
        matrix_segment,
        1,
        "resources/models/UI3.png",
        command
    );
    m_pointer->SetColor(1.0f, 0.0f, 0.0f);

    m_target = std::make_unique<UIComponent>();
    m_target->Init(
        texture_segment,
        2,
        matrix_segment,
        2,
        "resources/models/UI4.png",
        command
    );
    m_target->SetScale(0.25f, 0.4f);

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
               .SrcBlendAlpha = D3D12_BLEND_ZERO,
               .DestBlendAlpha = D3D12_BLEND_ZERO,
               .BlendOpAlpha = D3D12_BLEND_OP_ADD,
               .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL}}};
    m_pipelineState.SetBlendState(blendDesc);
    m_pipelineState.SetDepthEnable(false);
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

    m_guide->Render(command);
    m_target->Render(command);
    m_pointer->Render(command);
}
