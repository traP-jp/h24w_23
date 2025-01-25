#ifndef TITLEIMAGE_H
#define TITLEIMAGE_H
#include <d2d1_3.h>
#include <wincodec.h>
#include <wrl/client.h>

class TitleImage
{
public:
    void Init(
        const Microsoft::WRL::ComPtr<ID2D1DeviceContext2> &d2dDeviceContext,
        D2D1_RECT_F imageRc,
        D2D1_RECT_F logoRc
    );
    void Render(const Microsoft::WRL::ComPtr<ID2D1DeviceContext2> &d2dDeviceContext) const;

    TitleImage()
        : pWICFactory(nullptr)
        , pBackgroundDecoder(nullptr)
        , pBackgroundSource(nullptr)
        , pBackgroundConverter(nullptr)
        , pBackgroundBitmap(nullptr)
        , pLogoDecoder(nullptr)
        , pLogoSource(nullptr)
        , pLogoConverter(nullptr)
        , pLogoBitmap(nullptr)
        , m_imageRc()
        , m_logoRc()
    {
    }

private:
    IWICImagingFactory *pWICFactory;

    IWICBitmapDecoder *pBackgroundDecoder;
    IWICBitmapFrameDecode *pBackgroundSource;
    IWICFormatConverter *pBackgroundConverter;
    ID2D1Bitmap1 *pBackgroundBitmap;

    IWICBitmapDecoder *pLogoDecoder;
    IWICBitmapFrameDecode *pLogoSource;
    IWICFormatConverter *pLogoConverter;
    ID2D1Bitmap1 *pLogoBitmap;

    D2D1_RECT_F m_imageRc;
    D2D1_RECT_F m_logoRc;
};

#endif  // TITLEIMAGE_H
