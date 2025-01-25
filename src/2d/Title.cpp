#include "2d/Title.h"

#include <DWrite.h>

void Title::Init(
    const Microsoft::WRL::ComPtr<IDWriteFactory>& dwriteFactory,
    const D2D1_RECT_F& textRect
)
{
    m_textRect = textRect;

    HRESULT hr = dwriteFactory->CreateTextFormat(
        L"Cascadia Code",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        72.0f,
        L"en-US",
        &m_textFormat
    );
    if (FAILED(hr))
    {
        OutputDebugString("Failed to create DWrite text format\n");
        return;
    }

    hr = m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    if (FAILED(hr))
    {
        OutputDebugString("Failed to set text alignment\n");
        return;
    }

    hr = m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    if (FAILED(hr))
    {
        OutputDebugString("Failed to set paragraph alignment\n");
        return;
    }
}

void Title::Render(
    const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
    const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush
) const
{
    d2dDeviceContext
        ->DrawTextA(m_text, wcslen(m_text), m_textFormat.Get(), m_textRect, brush.Get());
}
