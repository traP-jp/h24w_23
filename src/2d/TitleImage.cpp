#include "2d/TitleImage.h"

#include <AquaEngine.h>
#include <wincodec.h>

void TitleImage::Init(
    const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
    D2D1_RECT_F imageRc,
    D2D1_RECT_F logoRc
)
{
    m_imageRc = imageRc;
    m_logoRc = logoRc;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&pWICFactory)
    );
    if (FAILED(hr))
    {
        OutputDebugString("failed to create WIC factory\n");
        return;
    }

    hr = pWICFactory->CreateDecoderFromFilename(
        L"resources/models/title3.png",
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pBackgroundDecoder
    );
    if (FAILED(hr))
    {
        OutputDebugString("failed to create decoder\n");
        return;
    }

    hr = pBackgroundDecoder->GetFrame(0, &pBackgroundSource);
    if (FAILED(hr))
    {
        OutputDebugString("failed to get frame\n");
        return;
    }

    hr = pWICFactory->CreateFormatConverter(&pBackgroundConverter);
    if (FAILED(hr))
    {
        OutputDebugString("failed to create format converter\n");
        return;
    }

    hr = pBackgroundConverter->Initialize(
        pBackgroundSource,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeCustom
    );
    if (FAILED(hr))
    {
        OutputDebugString("failed to initialize converter\n");
        return;
    }

    AquaEngine::SafeRelease(&pBackgroundDecoder);

    hr = d2dDeviceContext
             ->CreateBitmapFromWicBitmap(pBackgroundConverter, nullptr, &pBackgroundBitmap);
    if (FAILED(hr))
    {
        OutputDebugString("failed to create bitmap\n");
        return;
    }

    hr = pWICFactory->CreateDecoderFromFilename(
        L"resources/models/titlelogo.png",
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pLogoDecoder
    );
    if (FAILED(hr))
    {
        OutputDebugString("failed to create decoder\n");
        return;
    }

    hr = pLogoDecoder->GetFrame(0, &pLogoSource);
    if (FAILED(hr))
    {
        OutputDebugString("failed to get frame\n");
        return;
    }

    hr = pWICFactory->CreateFormatConverter(&pLogoConverter);
    if (FAILED(hr))
    {
        OutputDebugString("failed to create format converter\n");
        return;
    }

    hr = pLogoConverter->Initialize(
        pLogoSource,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeCustom
    );
    if (FAILED(hr))
    {
        OutputDebugString("failed to initialize converter\n");
        return;
    }

    hr = d2dDeviceContext->CreateBitmapFromWicBitmap(pLogoConverter, nullptr, &pLogoBitmap);
    if (FAILED(hr))
    {
        OutputDebugString("failed to create bitmap\n");
        return;
    }
}

void TitleImage::Render(const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext) const
{
    d2dDeviceContext->DrawBitmap(pBackgroundBitmap, m_imageRc);
    d2dDeviceContext->DrawBitmap(pLogoBitmap, m_logoRc);
}
