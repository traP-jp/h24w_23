#include "2d/D2DEngine.h"

#include <directx/wrapper/Device.h>

D2DEngine::D2DEngine(HWND hwnd, RECT wr, AquaEngine::Command *command)
    : m_hwnd(hwnd), m_wr(wr), command(command), m_title()
{
}

void D2DEngine::Init(UINT back_buffer_count, const std::vector<ID3D12Resource *> &back_buffers)
{
    HRESULT hr = D3D11On12CreateDevice(
        AquaEngine::Device::Get().Get(),
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        reinterpret_cast<IUnknown **>(command->Queue().GetAddressOf()),
        1,
        0,
        &m_d3d11Device,
        &m_d3d11DeviceContext,
        nullptr
    );
    if (FAILED(hr))
    {
        std::println("Failed to create D3D11On12 device");
        return;
    }

    hr = m_d3d11Device.As(&m_d3d11On12Device);
    if (FAILED(hr))
    {
        std::println("Failed to get D3D11On12 device");
        return;
    }

    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(m_d2dFactory),
        nullptr,
        &m_d2dFactory
    );
    if (FAILED(hr))
    {
        std::println("Failed to create D2D factory");
        return;
    }

    hr = m_d3d11On12Device.As(&m_dxgiDevice);
    if (FAILED(hr))
    {
        std::println("Failed to get DXGI device");
        return;
    }

    hr = m_d2dFactory->CreateDevice(m_dxgiDevice.Get(), &m_d2dDevice);
    if (FAILED(hr))
    {
        std::println("Failed to create D2D device");
        return;
    }

    hr = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dDeviceContext);
    if (FAILED(hr))
    {
        std::println("Failed to create D2D device context");
        return;
    }

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_dwriteFactory),
        reinterpret_cast<IUnknown **>(m_dwriteFactory.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::println("Failed to create DWrite factory");
        return;
    }

    m_wrappedBackBuffers.resize(back_buffer_count);
    m_d2dBackBufferRenderTargets.resize(back_buffer_count);
    D3D11_RESOURCE_FLAGS flags = {D3D11_BIND_RENDER_TARGET};
    for (int i = 0; i < back_buffer_count; ++i)
    {
        hr = m_d3d11On12Device->CreateWrappedResource(
            back_buffers[i],
            &flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&m_wrappedBackBuffers[i])
        );
        if (FAILED(hr))
        {
            std::println("Failed to create wrapped resource");
            return;
        }

        Microsoft::WRL::ComPtr<IDXGISurface> surface;
        hr = m_wrappedBackBuffers[i].As(&surface);
        if (FAILED(hr))
        {
            std::println("Failed to get IDXGISurface");
            return;
        }

        hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
            surface.Get(),
            nullptr,
            &m_d2dBackBufferRenderTargets[i]
        );
        if (FAILED(hr))
        {
            std::println("Failed to create D2D bitmap");
            return;
        }
    }

    hr = m_d2dDeviceContext->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &m_d2dBlackBrush
    );
    if (FAILED(hr))
    {
        std::println("Failed to create D2D brush");
        return;
    }

    hr = m_d2dDeviceContext->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &m_d2dWhiteBrush
    );
    if (FAILED(hr))
    {
        std::println("Failed to create D2D brush");
        return;
    }

    m_title.Init(
        m_dwriteFactory,
        D2D1::RectF(0, m_wr.bottom - 300, m_wr.right - m_wr.left, m_wr.bottom)
    );

    m_titleImage.Init(
        m_d2dDeviceContext,
        D2D1::RectF(0, 0, m_wr.right - m_wr.left, m_wr.bottom - m_wr.top),
        D2D1::RectF(0, 170, m_wr.right - m_wr.left - 100, 870)
    );

    m_gameInfo.Init(
        m_dwriteFactory,
        D2D1::RectF(50, 50, 700, 100),
        D2D1::RectF(50, 100, 700, 150),
        D2D1::RectF(50, 150, 700, 200),
        D2D1::RectF(50, 200, 700, 250)
    );
}

void D2DEngine::BeginRender(UINT back_buffer_index)
{
    m_d3d11On12Device->AcquireWrappedResources(
        m_wrappedBackBuffers[back_buffer_index].GetAddressOf(),
        1
    );

    m_d2dDeviceContext->SetTarget(m_d2dBackBufferRenderTargets[back_buffer_index].Get());
    m_d2dDeviceContext->BeginDraw();
    m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DEngine::EndRender(UINT back_buffer_index)
{
    HRESULT hr = m_d2dDeviceContext->EndDraw();
    if (FAILED(hr))
    {
        std::println("Failed to draw text");
        return;
    }

    m_d3d11On12Device->ReleaseWrappedResources(
        m_wrappedBackBuffers[back_buffer_index].GetAddressOf(),
        1
    );
    m_d3d11DeviceContext->Flush();
}

// clear the back buffer and render the title text
void D2DEngine::RenderTitle(UINT back_buffer_index)
{
    BeginRender(back_buffer_index);
    m_d2dDeviceContext->Clear(m_titleBackgroundColor);
    m_titleImage.Render(m_d2dDeviceContext);
    m_title.Render(m_d2dDeviceContext, m_d2dWhiteBrush);
    EndRender(back_buffer_index);
}

void D2DEngine::RenderTitleTextRenderTarget()
{
    BeginRenderRenderTarget();
    m_d2dDeviceContext->Clear(m_titleBackgroundColor);
    m_titleImage.Render(m_d2dDeviceContext);
    m_title.Render(m_d2dDeviceContext, m_d2dBlackBrush);
    EndRenderRenderTarget();
}

void D2DEngine::RenderGameInfo(UINT back_buffer_index)
{
    BeginRender(back_buffer_index);
    m_gameInfo.Render(m_d2dDeviceContext, m_d2dWhiteBrush);
    EndRender(back_buffer_index);
}

void D2DEngine::BeginRenderRenderTarget()
{
    m_d3d11On12Device->AcquireWrappedResources(m_wrappedRenderTarget.GetAddressOf(), 1);

    m_d2dDeviceContext->SetTarget(m_d2dRenderTarget.Get());
    m_d2dDeviceContext->BeginDraw();
    m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DEngine::EndRenderRenderTarget()
{
    HRESULT hr = m_d2dDeviceContext->EndDraw();
    if (FAILED(hr))
    {
        std::println("Failed to draw text");
        return;
    }

    m_d3d11On12Device->ReleaseWrappedResources(m_wrappedRenderTarget.GetAddressOf(), 1);
    m_d3d11DeviceContext->Flush();
}
