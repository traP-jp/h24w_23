#ifndef D2DENGINE_H
#define D2DENGINE_H

#include <AquaEngine.h>
#include <d2d1_3.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dwrite_3.h>
#include <windows.h>
#include <wrl/client.h>

#include <vector>

#include "Title.h"

class D2DEngine
{
public:
    D2DEngine(HWND hwnd, RECT wr, AquaEngine::Command* command);

    void Init(
        UINT back_buffer_count,
        const std::vector<ID3D12Resource*>& back_buffers
    );
    void RenderTitleText(UINT back_buffer_index);

private:
    void BeginRender(UINT back_buffer_index);
    void EndRender(UINT back_buffer_index);

    HWND m_hwnd;
    RECT m_wr;

    AquaEngine::Command* command;

    Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
    Microsoft::WRL::ComPtr<ID3D11On12Device> m_d3d11On12Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
    std::vector<Microsoft::WRL::ComPtr<ID3D11Resource>> m_wrappedBackBuffers;

    Microsoft::WRL::ComPtr<ID2D1Factory3> m_d2dFactory;
    Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice;
    Microsoft::WRL::ComPtr<ID2D1Device2> m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_d2dDeviceContext;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_d2dBlackBrush;
    std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_d2dRenderTargets;

    Microsoft::WRL::ComPtr<IDWriteFactory> m_dwriteFactory;

    Title m_title;
};

#endif  // D2DENGINE_H
