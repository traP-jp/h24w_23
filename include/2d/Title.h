#ifndef TITLE_H
#define TITLE_H

#include <d2d1_3.h>
#include <wrl/client.h>

class Title
{
public:
    Title() = default;

    void Init(
        const Microsoft::WRL::ComPtr<IDWriteFactory>& dwriteFactory,
        const D2D1_RECT_F& textRect
    );

    void Render(
        const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
        const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush
    ) const;

private:
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

    D2D1_RECT_F m_textRect;

    const wchar_t* m_text = L"Press any button to start!";
};

#endif  // TITLE_H
