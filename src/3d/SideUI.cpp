#include "3d/SideUI.h"

void SideUI::Init(AquaEngine::Command& command)
{
    ImportModel(command);

    auto& manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "side_ui",
        10,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    auto matrix_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_model->CreateMatrixBuffer(std::move(matrix_range), manager);

    auto material_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_model->CreateMaterialBufferView(std::move(material_range), manager);

    m_alpha1Texture = std::make_unique<AquaEngine::Buffer>(
        AquaEngine::TextureManager::LoadTextureFromFile("resources/models/UI2_alpha1.png", command)
    );
    auto alpha1_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    auto alpha1_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(manager.Allocate(1));
    alpha1_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_PIXEL,
        std::move(alpha1_range),
        1
    );
    m_alpha1SRV.SetDescriptorHeapSegment(alpha1_segment, 0);
    m_alpha1SRV.Create(*m_alpha1Texture);

    m_alpha2Texture = std::make_unique<AquaEngine::Buffer>(
        AquaEngine::TextureManager::LoadTextureFromFile("resources/models/UI2_alpha2.png", command)
    );
    auto alpha2_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    auto alpha2_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(manager.Allocate(1));
    alpha2_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_PIXEL,
        std::move(alpha2_range),
        1
    );
    m_alpha2SRV.SetDescriptorHeapSegment(alpha2_segment, 0);
    m_alpha2SRV.Create(*m_alpha2Texture);

    HRESULT hr = m_scrollBuffer.Create(BUFFER_DEFAULT(
        AquaEngine::AlignmentSize(sizeof(Scroll), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)
    ));
    if (FAILED(hr))
    {
        std::cout << "failed to create scroll buffer" << std::endl;
        exit(-1);
    }
    m_scrollBuffer.GetMappedBuffer()->scroll = m_scroll;
    auto scroll_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        3,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    auto scroll_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(manager.Allocate(1));
    scroll_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(scroll_range),
        1
    );
    m_scrollCBV.SetDescriptorHeapSegment(scroll_segment, 0);
    m_scrollCBV.Create(m_scrollBuffer.GetBuffer());
}

void SideUI::CreatePipelineState()
{
    m_rootSignature.AddStaticSampler(AquaEngine::RootSignature::DefaultStaticSampler());
    auto& manager = AquaEngine::GlobalDescriptorHeapManager::GetShaderHeapManager(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        "side_ui"
    );
    m_rootSignature.SetDescriptorHeapSegmentManager(&manager);
    HRESULT hr = m_rootSignature.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create root signature" << std::endl;
        exit(-1);
    }

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"shaders/side_ui.hlsl", "vs", "vs_5_0");
    ps.Load(L"shaders/side_ui.hlsl", "ps", "ps_5_0");

    auto inputElement = m_model->GetInputElementDescs();

    m_pipelineState.SetRootSignature(&m_rootSignature);
    m_pipelineState.SetVertexShader(&vs);
    m_pipelineState.SetPixelShader(&ps);
    m_pipelineState.SetInputLayout(inputElement.data(), inputElement.size());
    D3D12_BLEND_DESC blend
        = {.AlphaToCoverageEnable = false,
           .IndependentBlendEnable = false,
           .RenderTarget
           = {{.BlendEnable = true,
               .SrcBlend = D3D12_BLEND_SRC_ALPHA,
               .DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
               .BlendOp = D3D12_BLEND_OP_ADD,
               .SrcBlendAlpha = D3D12_BLEND_ONE,
               .DestBlendAlpha = D3D12_BLEND_ZERO,
               .BlendOpAlpha = D3D12_BLEND_OP_ADD,
               .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL}}};
    m_pipelineState.SetBlendState(blend);
    m_pipelineState.SetDepthEnable(false);
    hr = m_pipelineState.Create();
    if (FAILED(hr))
    {
        std::cout << "failed to create pipeline state" << std::endl;
        exit(-1);
    }
}

void SideUI::UseRootSignature(AquaEngine::Command& command) const
{
    m_pipelineState.SetToCommand(command);
    m_rootSignature.SetToCommand(command);
}

void SideUI::Render(AquaEngine::Command& command)
{
    m_alpha1SRV.SetGraphicsRootDescriptorTable(&command);
    m_alpha2SRV.SetGraphicsRootDescriptorTable(&command);
    m_scrollCBV.SetGraphicsRootDescriptorTable(&command);
    m_model->Render(command);
}

void SideUI::ImportModel(AquaEngine::Command& command)
{
    m_model = std::make_unique<AquaEngine::FBXModel>("resources/models/UI2_fbx.fbx");
    m_model->Create();
}
