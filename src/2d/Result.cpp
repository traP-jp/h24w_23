#include "2d/Result.h"

void Result::Init(AquaEngine::Command& command)
{
    m_resultWinText = std::make_unique<AquaEngine::RectangleTexture>(
        DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
        "resources/models/result_win.png",
        command
    );
    m_resultWinText->Create();

    m_resultLoseText = std::make_unique<AquaEngine::RectangleTexture>(
        DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
        DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),
        DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
        "resources/models/result_lose.png",
        command
    );
    m_resultLoseText->Create();

    m_background = std::make_unique<AquaEngine::Rectangle>(
        DirectX::XMFLOAT3(-0.5f, 0.3f, 0.0f),
        DirectX::XMFLOAT3(0.5f, 0.3f, 0.0f),
        DirectX::XMFLOAT3(-0.5f, -0.3f, 0.0f),
        DirectX::XMFLOAT3(0.5f, -0.3f, 0.0f)
    );

    auto& text_manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "text",
        1,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    auto text_texture_segment
        = std::make_shared<AquaEngine::DescriptorHeapSegment>(text_manager.Allocate(1));
    auto text_texture_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    text_texture_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(text_texture_range),
        1
    );

    auto& back_manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "background",
        1,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    auto back_texture_segment
        = std::make_shared<AquaEngine::DescriptorHeapSegment>(back_manager.Allocate(1));
    auto back_texture_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    back_texture_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(back_texture_range),
        1
    );

    auto input_text = AquaEngine::RectangleTexture::GetInputElementDescs();
    auto input_back = AquaEngine::Rectangle::GetInputElementDescs();

    m_textRootSignature.AddStaticSampler(AquaEngine::RootSignature::DefaultStaticSampler());
    m_textRootSignature.SetDescriptorHeapSegmentManager(&text_manager);
    HRESULT hr = m_textRootSignature.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create text root signature" << std::endl;
        exit(-1);
    }

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"resources/shaders/resulttext.hlsl", "main", "vs_5_0");
    ps.Load(L"resources/shaders/resulttext.hlsl", "main", "ps_5_0");
    m_textPipelineState.SetRootSignature(&m_textRootSignature);
    m_textPipelineState.SetVertexShader(&vs);
    m_textPipelineState.SetPixelShader(&ps);
    m_textPipelineState.SetInputLayout(input_text.data(), input_text.size());
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
    m_textPipelineState.SetBlendState(blendDesc);
    m_textPipelineState.SetDepthEnable(false);
    hr = m_textPipelineState.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create text pipeline state" << std::endl;
        exit(-1);
    }

    m_backgroundRootSignature.SetDescriptorHeapSegmentManager(&back_manager);
    hr = m_backgroundRootSignature.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create background root signature" << std::endl;
        exit(-1);
    }

    AquaEngine::ShaderObject back_vs, back_ps;
    back_vs.Load(L"resources/shaders/resultback.hlsl", "main", "vs_5_0");
    back_ps.Load(L"resources/shaders/resultback.hlsl", "main", "ps_5_0");
    m_backgroundPipelineState.SetRootSignature(&m_backgroundRootSignature);
    m_backgroundPipelineState.SetVertexShader(&back_vs);
    m_backgroundPipelineState.SetPixelShader(&back_ps);
    m_backgroundPipelineState.SetInputLayout(input_back.data(), input_back.size());
    m_backgroundPipelineState.SetBlendState(blendDesc);
    m_backgroundPipelineState.SetDepthEnable(false);
    hr = m_backgroundPipelineState.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create background pipeline state" << std::endl;
        exit(-1);
    }
}

void Result::Render(AquaEngine::Command& command, bool win)
{
    m_backgroundRootSignature.SetToCommand(command);
    m_backgroundPipelineState.SetToCommand(command);
    m_background->Render(command);

    m_textRootSignature.SetToCommand(command);
    m_textPipelineState.SetToCommand(command);
    if (win)
    {
        m_resultWinText->Render(command);
    }
    else
    {
        m_resultLoseText->Render(command);
    }
}
