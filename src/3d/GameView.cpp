#include "3d/GameView.h"

GameView::GameView(HWND hwnd, RECT rc) : m_hwnd(hwnd), m_camera(rc)
{
}

void GameView::Init(AquaEngine::Command &command)
{
    auto &manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "main_game",
            10,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera.Init({0.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    m_camera.AddManager("main_game", std::move(camera_range));

    auto matrix_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(2)
    );
    auto matrix_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    matrix_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(matrix_range),
        1
    );

    auto texture_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(2)
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

    auto material_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(2)
    );
    auto material_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        2,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    material_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(material_range),
        1
    );

    m_model1 = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/ninja.fbx",
        "resources/models/ninja.png",
        command
    );
    m_model1->Create();
    m_model1->CreateMatrixBuffer(matrix_segment, 0);
    m_model1->SetTexture(texture_segment, 0);
    m_model1->CreateMaterialBufferView(material_segment, 0);

    m_model2 = std::make_unique<AquaEngine::FBXModel>(
        "resources/models/isu.fbx",
        "resources/models/isu.png",
        command
    );
    m_model2->Create();
    m_model2->CreateMatrixBuffer(matrix_segment, 1);
    m_model2->SetTexture(texture_segment, 1);
    m_model2->CreateMaterialBufferView(material_segment, 1);

    auto model_input_element = m_model1->GetInputElementDescs();

    m_rootSignature.AddStaticSampler(
        AquaEngine::RootSignature::DefaultStaticSampler()
    );
    m_rootSignature.SetDescriptorHeapSegmentManager(&manager);
    HRESULT hr = m_rootSignature.Create();
    if (FAILED(hr))
    {
        std::println("failed to create root signature");
        exit(-1);
    }

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"shaders/main.hlsl", "vs", "vs_5_0");
    ps.Load(L"shaders/main.hlsl", "ps", "ps_5_0");

    m_pipelineState.SetRootSignature(&m_rootSignature);
    m_pipelineState.SetVertexShader(&vs);
    m_pipelineState.SetPixelShader(&ps);
    m_pipelineState.SetInputLayout(
        model_input_element.data(),
        model_input_element.size()
    );
    hr = m_pipelineState.Create();
    if (FAILED(hr))
    {
        std::println("failed to create pipeline state");
        exit(-1);
    }

    m_model1->PlayAnimation(
        "metarig|hirou",
        AquaEngine::FBXModel::AnimationMode::LOOP
    );

    m_model1->RotX(-DirectX::XM_PIDIV2);
    m_model1->RotY(DirectX::XM_PI);
    m_model2->RotX(-DirectX::XM_PIDIV2);

    m_model1->Move(-1.5f, -1.0f, 0.0f);
    m_model2->Move(1.5f, -1.0f, 0.0f);

    m_model1->Scale(2.0f, 2.0f, 2.0f);
    m_model2->Scale(2.0f, 2.0f, 2.0f);
}

void GameView::Render(AquaEngine::Command &command)
{
    m_model2->RotY(-0.1f);

    m_rootSignature.SetToCommand(command);
    m_pipelineState.SetToCommand(command);

    m_camera.Render(command, "main_game");
    m_model1->Render(command);
    m_model2->Render(command);
}

void GameView::Timer(int id) const
{
    switch (id)
    {
        case TIMER_MODEL1:
            m_model1->Timer();
            break;

        case TIMER_MODEL2:
            m_model2->Timer();
            break;

        default:
            break;
    }
}

void GameView::StartAnimation()
{
    SetTimer(m_hwnd, TIMER_MODEL1, m_model1->GetFrameCount(), nullptr);
    SetTimer(m_hwnd, TIMER_MODEL2, m_model2->GetFrameCount(), nullptr);
}
