#include "3d/Engine.h"

#include <iostream>

Engine::Engine(HWND hwnd, RECT wr) : m_hwnd(hwnd), m_wr(wr)
{
    AquaEngine::Factory::Init(true);
    AquaEngine::Device::GetAdaptors();
    AquaEngine::Device::Init(0);
    AquaEngine::GlobalDescriptorHeapManager::Init();
    AquaEngine::FBXManager::Init();
}

Engine::~Engine()
{
    AquaEngine::FBXManager::Shutdown();
    AquaEngine::GlobalDescriptorHeapManager::Shutdown();
    AquaEngine::Device::Shutdown();
    AquaEngine::Factory::Shutdown();
}

void Engine::Init()
{
#ifdef DEBUG
    m_startStatus = StartStatus::RUNNING;
#endif

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        std::println("Failed to initialize COM");
        return;
    }

    m_command = std::make_unique<AquaEngine::Command>();
    m_display = std::make_unique<AquaEngine::Display>(m_hwnd, m_wr, *m_command);
    m_display->SetBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

    InitRenderTargets();

#ifndef DEBUG
    m_d2dEngine = std::make_unique<D2DEngine>(m_hwnd, m_wr, m_command.get());
    auto desc = m_display->GetSwapChainDesc();
    m_d2dEngine->Init(desc.BufferCount, m_display->GetBackBufferResouces());
#endif

    m_gameView = std::make_unique<GameView>(m_hwnd, m_wr);
    m_gameView->Init(*m_command);

    std::cout << "Engine initialized" << std::endl;

#ifdef DEBUG
    m_gameView->Start();
#endif
}

void Engine::InitRenderTargets()
{
    m_gameRenderTarget.SetBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);
    HRESULT hr = m_gameRenderTarget.Create(
        m_display->GetBackBufferResourceDesc(),
        m_wr.right - m_wr.left,
        m_wr.bottom - m_wr.top
    );
    if (FAILED(hr))
    {
        std::println("Failed to create game render target");
        return;
    }

    auto &render_target_manager
        = AquaEngine::GlobalDescriptorHeapManager::CreateShaderManager(
            "render_target",
            10,
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

    auto game_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        render_target_manager.Allocate(1)
    );
    auto game_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    game_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_PIXEL,
        std::move(game_range),
        1
    );
    hr = m_gameRenderTarget.CreateShaderResourceView(game_segment, 0);
    if (FAILED(hr))
    {
        std::println("Failed to create game render target SRV");
        return;
    }

    m_weightBuffer.Create(BUFFER_DEFAULT(
        AquaEngine::AlignmentSize(
            sizeof(Weight),
            D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT
        )
    ));
    m_weightBuffer.GetMappedBuffer()->gameWeight = m_gameWeight;
    auto weight_segment = std::make_shared<AquaEngine::DescriptorHeapSegment>(
        render_target_manager.Allocate(1)
    );
    auto weight_range = std::make_unique<D3D12_DESCRIPTOR_RANGE>(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0,
        0,
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
    );
    weight_segment->SetRootParameter(
        D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        D3D12_SHADER_VISIBILITY_PIXEL,
        std::move(weight_range),
        1
    );
    m_weightCBV.SetDescriptorHeapSegment(weight_segment, 0);
    m_weightCBV.Create(m_weightBuffer.GetBuffer());

    m_rootSignature.AddStaticSampler(
        AquaEngine::RootSignature::DefaultStaticSampler()
    );
    m_rootSignature.SetDescriptorHeapSegmentManager(&render_target_manager);
    hr = m_rootSignature.Create();
    if (FAILED(hr))
    {
        std::println("Failed to create root signature");
        return;
    }
    auto render_targets_input_element
        = AquaEngine::RenderTarget::GetInputElementDescs();

    AquaEngine::ShaderObject vs, ps;
    vs.Load(L"shaders/renderTarget.hlsl", "vs", "vs_5_0");
    ps.Load(L"shaders/renderTarget.hlsl", "ps", "ps_5_0");

    m_pipelineState.SetRootSignature(&m_rootSignature);
    m_pipelineState.SetVertexShader(&vs);
    m_pipelineState.SetPixelShader(&ps);
    m_pipelineState.SetInputLayout(
        render_targets_input_element.data(),
        render_targets_input_element.size()
    );
    hr = m_pipelineState.Create();
    if (FAILED(hr))
    {
        std::println("Failed to create pipeline state");
        return;
    }
}

void Engine::Render()
{
    AquaEngine::GlobalDescriptorHeapManager::SetToCommand(*m_command);

    switch (m_startStatus)
    {
        case StartStatus::TITLE:
        {
            m_display->BeginRender();
            m_display->SetViewports();

            HRESULT hr = m_command->Execute();
            if (FAILED(hr))
            {
                std::println("Failed to execute command");
                return;
            }

            m_d2dEngine->RenderTitleText(m_display->GetCurrentBackBufferIndex()
            );

            m_display->Present();
            break;
        }
        case StartStatus::STARTING:
        {
            if (m_elapsedTime < 1.0f)
            {
                float weight = 1.0f - m_elapsedTime;
                m_d2dEngine->SetTitleBackgroundColor(
                    D2D1::ColorF(weight, weight, weight, 1.0f)
                );
                m_display->BeginRender();
                m_display->SetViewports();

                HRESULT hr = m_command->Execute();
                if (FAILED(hr))
                {
                    std::println("Failed to execute command");
                    return;
                }

                m_d2dEngine->RenderTitleText(
                    m_display->GetCurrentBackBufferIndex()
                );

                m_display->Present();
            }
            else if (m_elapsedTime < 2.0f)
            {
                m_display->BeginRender();
                m_display->SetViewports();
                m_display->EndRender();

                HRESULT hr = m_command->Execute();
                if (FAILED(hr))
                {
                    std::println("Failed to execute command");
                    return;
                }

                m_display->Present();
            }
            else
            {
                m_gameWeight = m_elapsedTime - 2.0f;
                m_weightBuffer.GetMappedBuffer()->gameWeight = m_gameWeight;

                m_gameRenderTarget.BeginRender(*m_command);

                m_display->SetViewports();
                m_gameView->Render(*m_command);

                m_gameRenderTarget.EndRender(*m_command);

                m_display->BeginRender();

                m_rootSignature.SetToCommand(*m_command);
                m_pipelineState.SetToCommand(*m_command);
                m_gameRenderTarget.UseAsTexture(*m_command);
                m_weightCBV.SetGraphicsRootDescriptorTable(m_command.get());
                m_display->SetViewports();
                m_gameRenderTarget.Render(*m_command);

                m_display->EndRender();

                HRESULT hr = m_command->Execute();
                if (FAILED(hr))
                {
                    std::println("Failed to execute command");
                    return;
                }

                m_display->Present();
            }
            break;
        }
        case StartStatus::RUNNING:
        {
            m_display->BeginRender();
            m_display->SetViewports();

            m_gameView->Render(*m_command);

            m_display->EndRender();

            HRESULT hr = m_command->Execute();
            if (FAILED(hr))
            {
                std::println("Failed to execute command");
                return;
            }

            m_display->Present();
            break;
        }
    }
}

void Engine::Timer(int id)
{
    switch (id)
    {
        case TIMER_TITLE:
            m_elapsedTime += 0.05f;
            if (m_elapsedTime >= 3.0f && m_startStatus != StartStatus::RUNNING)
            {
                m_startStatus = StartStatus::RUNNING;
                m_gameView->Start();
            }
            InvalidateRect(m_hwnd, &m_wr, FALSE);
            return;

        default:
            m_gameView->Timer(id);
            return;
    }
}
