#include "3d/GameView.h"

GameView::GameView(HWND hwnd, RECT rc)
    : m_hwnd(hwnd), m_rc(rc), m_isPlayer1(false)
{
}

void GameView::Init(AquaEngine::Command &command)
{
    auto &manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "main_game",
            100,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

    auto camera_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_camera = std::make_shared<AquaEngine::Camera>(m_rc);
    m_camera
        ->Init({0.0f, 5.0f, -10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 2.0f, 1.0f});
    m_camera->AddManager("main_game", std::move(camera_range));

    auto model_input_element = m_playerModel1.GetInputElementDescs();

    CreateModels(command, manager);
    CreateSkyBox(command);

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
}

void GameView::CreateModels(
    AquaEngine::Command &command,
    AquaEngine::DescriptorHeapSegmentManager &manager
)
{
    auto matrix_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(16)
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
        manager.Allocate(14)
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
        manager.Allocate(16)
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

    m_playerModel1.Init(command);
    m_playerModel1.SetMatrixSegments(matrix_segment, 0);
    m_playerModel1.SetTextureSegments(texture_segment, 0);
    m_playerModel1.SetMaterialSegments(material_segment, 0);

    m_playerModel2.Init(command);
    m_playerModel2.SetMatrixSegments(matrix_segment, 1);
    m_playerModel2.SetTextureSegments(texture_segment, 1);
    m_playerModel2.SetMaterialSegments(material_segment, 1);

    m_playerModel1.Scale(0.001f, 0.001f, 0.001f);
    m_playerModel2.Scale(0.001f, 0.001f, 0.001f);
    m_playerModel1.Move(-1.0f, 0.0f, 0.0f);
    m_playerModel2.Move(1.0f, 0.0f, 0.0f);
}

void GameView::CreateSkyBox(AquaEngine::Command &command)
{
    auto &skybox_manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "skybox",
            10,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

    m_skyBox = std::make_unique<AquaEngine::SkyBox>(
        "resources/textures/space.hdr",
        command,
        skybox_manager
    );
    auto world_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_skyBox->CreateMatrixBuffer(std::move(world_range), skybox_manager);
    m_skyBox->Create();
    m_skyBox->SetCamera(m_camera);
    m_skyBox->ConvertHDRIToCubeMap(command);
    m_skyBox->CreateCubeMapPipelineState();
    m_skyBox->Scale(1000.0f, 1000.0f, 1000.0f);
}

void GameView::Render(AquaEngine::Command &command)
{
    m_playerModel1.Rot(0.0f, 0.01f, 0.0f);

    m_skyBox->Render(command);

    m_rootSignature.SetToCommand(command);
    m_pipelineState.SetToCommand(command);

    m_camera->Render(command, "main_game");
    m_playerModel1.Render(command);
    m_playerModel2.Render(command);
}

void GameView::Timer(int id) const
{
    switch (id)
    {
        case TIMER_MODEL1:
            m_playerModel1.Timer();
            break;

        case TIMER_MODEL2:
            m_playerModel2.Timer();
            break;

        default:
            break;
    }
}

void GameView::StartAnimation()
{
    // SetTimer(m_hwnd, TIMER_MODEL1, m_playerModel1->GetFrameCount(), nullptr);
    // SetTimer(m_hwnd, TIMER_MODEL2, m_playerModel2->GetFrameCount(), nullptr);
}
