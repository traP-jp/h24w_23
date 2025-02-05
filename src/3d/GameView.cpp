#include "3d/GameView.h"

#include <random>

using DirectX::operator-;

GameView::GameView(HWND hwnd, RECT rc, bool is_player1)
    : m_hwnd(hwnd)
    , m_rc(rc)
    , m_isPlayer1(is_player1)
{
}

void GameView::Init(AquaEngine::Command &command)
{
    auto &manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "main_game",
        1000,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    m_camera = std::make_unique<Camera>(m_rc, m_isPlayer1);
    m_camera->Init(); // init ni camera range

    auto model_input_element = m_playerModel1.GetInputElementDescs();

    CreateModels(command, manager);
    CreateSkyBox(command);

    m_camera->InitBullet(); // init ni camera range
    m_camera->InitSideUI(); // init ni camera range
    m_sideUI.CreatePipelineState();

    auto light_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        3,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    m_directionLight.Init({0.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, manager, std::move(light_range));

    m_rootSignature.AddStaticSampler(AquaEngine::RootSignature::DefaultStaticSampler());
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
    m_pipelineState.SetInputLayout(model_input_element.data(), model_input_element.size());
    hr = m_pipelineState.Create();
    if (FAILED(hr))
    {
        std::println("failed to create pipeline state");
        exit(-1);
    }

    auto &bullet_manager = AquaEngine::GlobalDescriptorHeapManager::GetShaderHeapManager(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        "bullet"
    );
    m_bulletRootSignature.AddStaticSampler(AquaEngine::RootSignature::DefaultStaticSampler());
    m_bulletRootSignature.SetDescriptorHeapSegmentManager(&bullet_manager);
    hr = m_bulletRootSignature.Create();
    if (FAILED(hr))
    {
        std::println("failed to create bullet root signature");
        exit(-1);
    }

    AquaEngine::ShaderObject bullet_vs, bullet_ps;
    bullet_vs.Load(L"shaders/bullet.hlsl", "vs", "vs_5_0");
    bullet_ps.Load(L"shaders/bullet.hlsl", "ps", "ps_5_0");

    m_bulletPipelineState.SetRootSignature(&m_bulletRootSignature);
    m_bulletPipelineState.SetVertexShader(&bullet_vs);
    m_bulletPipelineState.SetPixelShader(&bullet_ps);
    m_bulletPipelineState.SetInputLayout(model_input_element.data(), model_input_element.size());
    hr = m_bulletPipelineState.Create();
    if (FAILED(hr))
    {
        std::println("failed to create bullet pipeline state");
        exit(-1);
    }

    m_effectManager.Init(command);
    m_effectManager.SetCamera(m_camera->GetCamera());

    m_playerModel1.CreateEffect(m_effectManager.GetManager());
    m_playerModel2.CreateEffect(m_effectManager.GetManager());

    CreateUI(command);

    auto m = AquaEngine::GlobalDescriptorHeapManager::GetShaderHeapManager(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        "side_ui"
    );
}

void GameView::CreateModels(
    AquaEngine::Command &command,
    AquaEngine::DescriptorHeapSegmentManager &manager
)
{
    auto matrix_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        manager.Allocate(16 + m_asteroids.size())
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
        manager.Allocate(14 + m_asteroids.size())
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
        manager.Allocate(16 + m_asteroids.size())
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

    auto &eye_manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "eye",
        10,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    auto eye_matrix_segment
        = std::make_shared<AquaEngine::DescriptorHeapSegment>(eye_manager.Allocate(2));
    auto eye_matrix_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    eye_matrix_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(eye_matrix_range),
        1
    );

    auto eye_material_segment
        = std::make_shared<AquaEngine::DescriptorHeapSegment>(eye_manager.Allocate(2));
    auto eye_material_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        2,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    eye_material_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(eye_material_range),
        1
    );

    m_playerModel1.SetEyeMatrixSegments(eye_matrix_segment, 0);
    m_playerModel1.SetEyeMaterialSegments(eye_material_segment, 0);
    m_playerModel2.SetEyeMatrixSegments(eye_matrix_segment, 1);
    m_playerModel2.SetEyeMaterialSegments(eye_material_segment, 1);

    m_camera->InitEye(); // init ni camera range

    m_eyeRootSignature.SetDescriptorHeapSegmentManager(&eye_manager);
    HRESULT hr = m_eyeRootSignature.Create();
    if (FAILED(hr))
    {
        std::println("failed to create eye root signature");
        exit(-1);
    }

    AquaEngine::ShaderObject eye_vs, eye_ps;
    eye_vs.Load(L"shaders/eye.hlsl", "vs", "vs_5_0");
    eye_ps.Load(L"shaders/eye.hlsl", "ps", "ps_5_0");

    auto input = m_playerModel1.GetInputElementDescs();

    m_eyePipelineState.SetRootSignature(&m_eyeRootSignature);
    m_eyePipelineState.SetVertexShader(&eye_vs);
    m_eyePipelineState.SetPixelShader(&eye_ps);
    m_eyePipelineState.SetInputLayout(input.data(), input.size());
    hr = m_eyePipelineState.Create();
    if (FAILED(hr))
    {
        std::println("failed to create eye pipeline state");
        exit(-1);
    }

    auto &bullet_manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "bullet",
        7 * Player::BULLET_COUNT,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    auto bullet_matrix_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        bullet_manager.Allocate(Player::BULLET_COUNT * 2)
    );
    auto bullet_matrix_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    bullet_matrix_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(bullet_matrix_range),
        1
    );

    auto bullet_material_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        bullet_manager.Allocate(Player::BULLET_COUNT * 2)
    );
    auto bullet_material_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        2,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    bullet_material_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(bullet_material_range),
        1
    );

    auto bullet_texture_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        bullet_manager.Allocate(Player::BULLET_COUNT * 2)
    );
    auto bullet_texture_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    bullet_texture_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_ALL,
        std::move(bullet_texture_range),
        1
    );

    m_playerModel1.SetBulletMatrixSegments(bullet_matrix_segment, 0);
    m_playerModel1.SetBulletMaterialSegments(bullet_material_segment, 0);
    m_playerModel1.SetBulletShaderResourceView(bullet_texture_segment, 0);
    m_playerModel2.SetBulletMatrixSegments(bullet_matrix_segment, 1);
    m_playerModel2.SetBulletMaterialSegments(bullet_material_segment, 1);
    m_playerModel2.SetBulletShaderResourceView(bullet_texture_segment, 1);

    m_playerModel1.Scale(Player::DEFAULT_SCALE, Player::DEFAULT_SCALE, Player::DEFAULT_SCALE);
    m_playerModel2.Scale(Player::DEFAULT_SCALE, Player::DEFAULT_SCALE, Player::DEFAULT_SCALE);
    m_playerModel1
        .Move(PLAYER1_DEFAULT_POSITION.x, PLAYER1_DEFAULT_POSITION.y, PLAYER1_DEFAULT_POSITION.z);
    m_playerModel2
        .Move(PLAYER2_DEFAULT_POSITION.x, PLAYER2_DEFAULT_POSITION.y, PLAYER2_DEFAULT_POSITION.z);

    m_bulletAlphaTexture
        = std::make_unique<AquaEngine::Buffer>(
            AquaEngine::TextureManager::LoadTextureFromFile(
                "resources/models/bullet_alpha.png",
                command
            )
        );
    auto bullet_alpha_texture_segment
        = std::make_shared<AquaEngine::DescriptorHeapSegment>(bullet_manager.Allocate(1));
    auto bullet_alpha_texture_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        1,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    bullet_alpha_texture_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_PIXEL,
        std::move(bullet_alpha_texture_range),
        1
    );
    m_bulletAlphaSRV.SetDescriptorHeapSegment(bullet_alpha_texture_segment, 0);
    m_bulletAlphaSRV.Create(*m_bulletAlphaTexture);

    std::mt19937 mt(std::random_device{}());
    std::gamma_distribution<> x_dist(2.0f, 3.1f);
    // std::gamma_distribution<> y_dist(2.0f, 2.9f);
    // std::gamma_distribution<> z_dist(1.8f, 7.0f);
    std::uniform_real_distribution<> y_dist(0.0f, 10.0f);
    std::uniform_real_distribution<> z_dist(0.0f, 10.0f);
    std::gamma_distribution<> scale_dist(1.0f, 3.0f);
    for (int i = 0; i < m_asteroids.size(); ++i)
    {
        m_asteroids[i].Init(command, i % 2 == 0);
        m_asteroids[i].SetMatrixSegments(matrix_segment, i + 16);
        m_asteroids[i].SetTextureSegments(texture_segment, i + 14);
        m_asteroids[i].SetMaterialSegments(material_segment, i + 16);
        m_asteroids[i].Scale(Player::DEFAULT_SCALE, Player::DEFAULT_SCALE, Player::DEFAULT_SCALE);
        float scale = scale_dist(mt) * 20.0f;
        if (scale > 200.0f)
        {
            scale = 600.0f;
        }
        m_asteroids[i].Scale(scale, scale, scale);
        m_asteroids[i].Move(
            x_dist(mt) * 100.0f - 500.0f,
            y_dist(mt) * 100.0f - 500.0f,
            z_dist(mt) * 100.0f - 500.0f
        );
    }

    m_playerModel1.PlayThurasterAction();
    m_playerModel1.PlayBusterAction();
    SetTimer(m_hwnd, TIMER_MODEL1, m_playerModel1.GetFrameCount(), nullptr);

    m_playerModel2.PlayThurasterAction();
    m_playerModel2.PlayBusterAction();
    SetTimer(m_hwnd, TIMER_MODEL2, m_playerModel2.GetFrameCount(), nullptr);

    m_sideUI.Init(command);
    m_sideUI.Scale(8.0f, 8.0f, 8.0f);
    // m_sideUI.Move(
    //     m_isPlayer1 ? PLAYER1_DEFAULT_POSITION.x : PLAYER2_DEFAULT_POSITION.x,
    //     m_isPlayer1 ? PLAYER1_DEFAULT_POSITION.y : PLAYER2_DEFAULT_POSITION.y,
    //     m_isPlayer1 ? PLAYER1_DEFAULT_POSITION.z : PLAYER2_DEFAULT_POSITION.z
    // );
}

void GameView::CreateSkyBox(AquaEngine::Command &command)
{
    auto &skybox_manager = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
        "skybox",
        // DONT CHANGE !!!!!
        10,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );

    m_skyBox = std::make_unique<AquaEngine::SkyBox>(
        "resources/textures/space.hdr",
        //"resources/textures/sample1.hdr",
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
    m_skyBox->SetCamera(m_camera->GetCamera());
    m_skyBox->ConvertHDRIToCubeMap(command);
    m_skyBox->CreateCubeMapPipelineState();
    m_skyBox->Scale(100000.0f, 100000.0f, 100000.0f);
}

void GameView::CreateUI(AquaEngine::Command &command)
{
    m_uiManager.Init(command);
    m_result.Init(command);
}

void GameView::Render(AquaEngine::Command &command)
{
    // m_playerModel1.RotY(0.01f);

    m_skyBox->Render(command);

    m_rootSignature.SetToCommand(command);
    m_pipelineState.SetToCommand(command);

    m_camera->Render(command);
    m_directionLight.Render(command);
    m_playerModel1.Render(command);
    m_playerModel2.Render(command);

    for (int i = 0; i < m_asteroids.size(); ++i)
    {
        m_asteroids[i].Render(command);
    }

    m_eyeRootSignature.SetToCommand(command);
    m_eyePipelineState.SetToCommand(command);
    m_camera->RenderEye(command);
    m_playerModel1.RenderEye(command);
    m_playerModel2.RenderEye(command);

    m_bulletRootSignature.SetToCommand(command);
    m_bulletPipelineState.SetToCommand(command);
    m_camera->RenderBullet(command);
    m_bulletAlphaSRV.SetGraphicsRootDescriptorTable(&command);
    m_playerModel1.RenderBullet(command);
    m_playerModel2.RenderBullet(command);

    m_sideUI.UseRootSignature(command);
    m_camera->RenderSideUI(command);
    m_sideUI.Render(command);

    m_uiManager.Render(command);

    if (m_gameStatus == GameStatus::WIN)
    {
        m_result.Render(command, true);
    } else if (m_gameStatus == GameStatus::LOSE)
    {
        m_result.Render(command, false);
    }

    m_effectManager.Render(command, m_camera->GetCamera());
}

void GameView::Timer(int id)
{
    switch (id)
    {
    case TIMER_MODEL1:
        m_playerModel1.Timer();
        break;

    case TIMER_MODEL2:
        m_playerModel2.Timer();
        break;

    case TIMER_FRAME:
    {
        (m_isPlayer1 ? m_playerModel1 : m_playerModel2).Frame();
        m_playerModel1.BulletFrame();
        m_playerModel2.BulletFrame();

        // camera, ui translation
        DirectX::XMVECTOR dr = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDrForCamera();
        DirectX::XMVECTOR dir = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection();
        DirectX::XMVECTOR up = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetUp();
        m_camera->StartFrame(dir, up);

        m_camera->Move(dr);
        m_camera->Accel((m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetAccelFrame());

        m_camera->EndFrame(dir, up);

        DirectX::XMMATRIX partner_transform
            = (m_isPlayer1 ? m_playerModel2 : m_playerModel1).GetMatrix();
        DirectX::XMVECTOR partner_position = partner_transform.r[3];
        // std::cout << "partner position: " << DirectX::XMVectorGetX(partner_position) << ", "
        //           << DirectX::XMVectorGetY(partner_position) << ", "
        //           << DirectX::XMVectorGetZ(partner_position) << std::endl;
        bool hit = (m_isPlayer1 ? m_playerModel1 : m_playerModel2)
            .IsHit(partner_position, m_effectManager.GetManager());
        if (hit)
        {
            std::cout << "hit" << std::endl;
            m_gameStatus = GameStatus::WIN;
            m_audioManager.RunTyakudanAudio(!m_isPlayer1);
        }

        DirectX::XMVECTOR my_position
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetMatrix().r[3];
        bool my_hit = (m_isPlayer1 ? m_playerModel2 : m_playerModel1)
            .IsHit(my_position, m_effectManager.GetManager());
        if (my_hit)
        {
            std::cout << "my hit" << std::endl;
            m_gameStatus = GameStatus::LOSE;
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).SetVelocity(0.0f);
            m_audioManager.RunTyakudanAudio(m_isPlayer1);
        }

        DirectX::XMVECTOR camera_y = m_camera->GetCamera()->GetUp();
        DirectX::XMVECTOR camera_z = m_camera->GetCamera()->GetTarget();
        camera_y = DirectX::XMVector3Normalize(camera_y);
        camera_z = DirectX::XMVector3Normalize(camera_z);
        DirectX::XMVECTOR camera_x = DirectX::XMVector3Cross(camera_y, camera_z);
        camera_x = DirectX::XMVector3Normalize(camera_x);
        DirectX::XMMATRIX camera_translate = DirectX::XMMatrixIdentity();
        camera_translate.r[0].m128_f32[0] = DirectX::XMVectorGetX(camera_x);
        camera_translate.r[1].m128_f32[0] = DirectX::XMVectorGetY(camera_x);
        camera_translate.r[2].m128_f32[0] = DirectX::XMVectorGetZ(camera_x);
        camera_translate.r[0].m128_f32[1] = DirectX::XMVectorGetX(camera_y);
        camera_translate.r[1].m128_f32[1] = DirectX::XMVectorGetY(camera_y);
        camera_translate.r[2].m128_f32[1] = DirectX::XMVectorGetZ(camera_y);
        camera_translate.r[0].m128_f32[2] = DirectX::XMVectorGetX(camera_z);
        camera_translate.r[1].m128_f32[2] = DirectX::XMVectorGetY(camera_z);
        camera_translate.r[2].m128_f32[2] = DirectX::XMVectorGetZ(camera_z);

        DirectX::XMVECTOR v = XMVector3Transform(
            XMVector3Transform(
                (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetDirection(),
                camera_translate
            ),
            m_camera->GetCamera()->GetProjection()
        );
        DirectX::XMVECTOR v0 = XMVector3Transform(
            XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), camera_translate),
            m_camera->GetCamera()->GetProjection()
        );
        DirectX::XMVECTOR pos_def
            = (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetMatrix().r[3];
        DirectX::XMFLOAT3 camera_pos = m_camera->GetCamera()->GetEye();
        DirectX::XMVECTOR pos = XMVector3Transform(
            XMVector3Transform(pos_def - XMLoadFloat3(&camera_pos), camera_translate),
            m_camera->GetCamera()->GetProjection()
        );

        // std::cout << "v - v0 : "
        //           << DirectX::XMVectorGetX(v) / DirectX::XMVectorGetW(v)
        //                  - DirectX::XMVectorGetX(v0)
        //           << ", "
        //           << DirectX::XMVectorGetY(v) / DirectX::XMVectorGetW(v)
        //                  - DirectX::XMVectorGetY(v0)
        //           << std::endl;
        // std::cout << "pos: " << DirectX::XMVectorGetX(pos) / DirectX::XMVectorGetW(pos) << ",
        // "
        //           << DirectX::XMVectorGetY(pos) / DirectX::XMVectorGetW(pos) << std::endl;
        m_uiManager.SetPointerPosition(
            DirectX::XMVectorGetX(v) / DirectX::XMVectorGetW(v) - DirectX::XMVectorGetX(v0)
            + DirectX::XMVectorGetX(pos) / DirectX::XMVectorGetW(pos),
            DirectX::XMVectorGetY(v) / DirectX::XMVectorGetW(v) - DirectX::XMVectorGetY(v0)
            + DirectX::XMVectorGetY(pos) / DirectX::XMVectorGetW(pos)
        );

        DirectX::XMVECTOR pertner_pos = XMVector3Transform(
            XMVector3Transform(partner_position - XMLoadFloat3(&camera_pos), camera_translate),
            // m_camera->GetCamera()->GetProjection()
            DirectX::XMMatrixIdentity()
        );
        DirectX::XMVECTOR pertner_vp = XMVector3Transform(
            DirectX::XMVector3Transform(partner_position, m_camera->GetCamera()->GetView()),
            m_camera->GetCamera()->GetProjection()
        );
        // std::cout << " partner pos: "
        //           << DirectX::XMVectorGetX(pertner_pos) / DirectX::XMVectorGetW(pertner_pos)
        //           << ", "
        //           << DirectX::XMVectorGetY(pertner_pos) / DirectX::XMVectorGetW(pertner_pos)
        //           //          << std::endl;
        //           // std::cout
        //           << " pertner position: "
        //           << DirectX::XMVectorGetX(partner_position - XMLoadFloat3(&camera_pos)) <<
        //           ", "
        //           << DirectX::XMVectorGetY(partner_position - XMLoadFloat3(&camera_pos)) <<
        //           ", "
        //           << DirectX::XMVectorGetZ(partner_position - XMLoadFloat3(&camera_pos))
        //           //           << std::endl;
        //           // std::cout
        //           << " camera pos: " << DirectX::XMVectorGetX(XMLoadFloat3(&camera_pos)) <<
        //           ","
        //           << DirectX::XMVectorGetY(XMLoadFloat3(&camera_pos)) << ", "
        //           << DirectX::XMVectorGetZ(XMLoadFloat3(&camera_pos)) << std::endl;
        // m_uiManager.SetTargetPosition(
        //     DirectX::XMVectorGetX(pertner_pos) / DirectX::XMVectorGetW(pertner_pos) + 0.5f,
        //     DirectX::XMVectorGetY(pertner_pos) / DirectX::XMVectorGetW(pertner_pos) + 0.5f
        // );
        float target_x
            = DirectX::XMVectorGetX(pertner_pos) / DirectX::XMVectorGetW(pertner_pos) * 2.0f;
        float target_y
            = DirectX::XMVectorGetY(pertner_pos) / DirectX::XMVectorGetW(pertner_pos) * 3.0f
              + 3.9f;
        if (abs(target_x) > 5.0f || abs(target_y) > 3.0f)
        {
            target_x /= max(abs(target_x) / 5.0f, abs(target_y) / 3.0f);
            target_y /= max(abs(target_x) / 5.0f, abs(target_y) / 3.0f);
            m_uiManager.SetTargetColor(1.0f, 0.0f, 0.0f);
            m_uiManager.SetTargetRotation(0.1f);
        } else
        {
            m_uiManager.SetTargetColor(0.5f, 0.5f, 1.0f);
            m_uiManager.SetTargetRotation(0.1f);
        }
        m_uiManager.SetTargetPosition(target_x, target_y);

        m_sideUI.SetTransformMatrix(
            (m_isPlayer1 ? m_playerModel1 : m_playerModel2).GetMatrix()
        );
        m_sideUI.Frame();

        // std::cout << (m_isPlayer1 ? "player1" : "player2") << std::endl;

        DirectX::XMVECTOR player1pos = m_playerModel1.GetPos();
        DirectX::XMVECTOR player2pos = m_playerModel2.GetPos();
        m_audioManager.SetPlayer1Pos(
            {
                DirectX::XMVectorGetX(player1pos),
                DirectX::XMVectorGetY(player1pos),
                DirectX::XMVectorGetZ(player1pos)
            }
        );
        m_audioManager.SetPlayer2Pos(
            {
                DirectX::XMVectorGetX(player2pos),
                DirectX::XMVectorGetY(player2pos),
                DirectX::XMVectorGetZ(player2pos)
            }
        );
        if (m_isPlayer1)
        {
            m_audioManager.SetListenerPos(
                {
                    DirectX::XMVectorGetX(player1pos),
                    DirectX::XMVectorGetY(player1pos),
                    DirectX::XMVectorGetZ(player1pos)
                }
            );
        } else
        {
            m_audioManager.SetListenerPos(
                {
                    DirectX::XMVectorGetX(player2pos),
                    DirectX::XMVectorGetY(player2pos),
                    DirectX::XMVectorGetZ(player2pos)
                }
            );
        }

        break;
    }

    default:
        break;
    }
}

void GameView::StartAnimation()
{
    SetTimer(m_hwnd, TIMER_MODEL1, m_playerModel1.GetFrameCount(), nullptr);
    SetTimer(m_hwnd, TIMER_MODEL2, m_playerModel2.GetFrameCount(), nullptr);
}

void GameView::Start()
{
    SetTimer(m_hwnd, TIMER_FRAME, 30, nullptr);
}
