#include "3d/Engine.h"

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
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        std::println("Failed to initialize COM");
        return;
    }

    m_command = std::make_unique<AquaEngine::Command>();
    m_display = std::make_unique<AquaEngine::Display>(m_hwnd, m_wr, *m_command);
    m_display->SetBackgroundColor(1.0f, 1.0f, 0.0f, 1.0f);

    m_d2dEngine = std::make_unique<D2DEngine>(m_hwnd, m_wr, m_command.get());
    auto desc = m_display->GetSwapChainDesc();
    m_d2dEngine->Init(desc.BufferCount, m_display->GetBackBufferResouces());
}

void Engine::Render() const
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

    m_d2dEngine->RenderTitleText(m_display->GetCurrentBackBufferIndex());

    m_display->Present();
}
