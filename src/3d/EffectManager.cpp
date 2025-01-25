#include "3d/EffectManager.h"

void EffectManager::Init(AquaEngine::Command& command)
{
    auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_manager = Effekseer::Manager::Create(80000);
    m_renderer = EffekseerRendererDX12::Create(
        AquaEngine::Device::Get().Get(),
        command.Queue().Get(),
        2,
        &format,
        1,
        DXGI_FORMAT_D32_FLOAT,
        false,
        8000
    );

    m_memoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer->GetGraphicsDevice());
    m_commandList
        = EffekseerRenderer::CreateCommandList(m_renderer->GetGraphicsDevice(), m_memoryPool);

    m_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

    m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
    m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
    m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
    m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
    m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());

    m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
    m_manager->SetModelLoader(m_renderer->CreateModelLoader());
    m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
    m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void EffectManager::SetCamera(const std::shared_ptr<AquaEngine::Camera>& camera)
{
    Effekseer::Matrix44 cameraMatrix;
    Effekseer::Matrix44 projMatrix;
    DirectX::XMMATRIX view = camera->GetView();
    DirectX::XMMATRIX proj = camera->GetProjection();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cameraMatrix.Values[i][j] = view.r[i].m128_f32[j];
            projMatrix.Values[i][j] = proj.r[i].m128_f32[j];
        }
    }
    m_renderer->SetCameraMatrix(cameraMatrix);
    m_renderer->SetProjectionMatrix(projMatrix);

    m_renderer->SetCommandList(m_commandList);
}

void EffectManager::UpdateCamera(const std::shared_ptr<AquaEngine::Camera>& camera)
{
    Effekseer::Matrix44 cameraMatrix;
    Effekseer::Matrix44 projMatrix;
    DirectX::XMMATRIX view = camera->GetView();
    DirectX::XMMATRIX proj = camera->GetProjection();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cameraMatrix.Values[i][j] = view.r[i].m128_f32[j];
            projMatrix.Values[i][j] = proj.r[i].m128_f32[j];
        }
    }

    m_renderer->SetCameraMatrix(cameraMatrix);
    m_renderer->SetProjectionMatrix(projMatrix);
}

void EffectManager::Render(
    AquaEngine::Command& command,
    const std::shared_ptr<AquaEngine::Camera>& camera
)
{
    m_memoryPool->NewFrame();

    EffekseerRendererDX12::BeginCommandList(m_commandList, command.List().Get());
    m_renderer->SetCommandList(m_commandList);

    DirectX::XMFLOAT3 eye = camera->GetEye();
    Effekseer::Manager::LayerParameter layer_parameter;
    Effekseer::Vector3D eye_position = {eye.x, eye.y, eye.z};
    layer_parameter.ViewerPosition = eye_position;
    m_manager->SetLayerParameter(0, layer_parameter);

    Effekseer::Manager::UpdateParameter update_parameter;
    m_manager->Update(update_parameter);

    UpdateCamera(camera);

    m_renderer->BeginRendering();

    Effekseer::Manager::DrawParameter draw_parameter;
    draw_parameter.ViewProjectionMatrix = m_renderer->GetCameraProjectionMatrix();
    m_manager->Draw(draw_parameter);

    m_renderer->EndRendering();

    m_renderer->SetCommandList(nullptr);
    EffekseerRendererDX12::EndCommandList(m_commandList);
}