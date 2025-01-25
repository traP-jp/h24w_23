#include "2d/GameInfo.h"

#include <DWrite.h>

void GameInfo::Init(
    const Microsoft::WRL::ComPtr<IDWriteFactory>& dwriteFactory,
    const D2D1_RECT_F& timeTextRect,
    const D2D1_RECT_F& velocityTextRect
)
{
    m_timeTextRect = timeTextRect;
    m_velocityTextRect = velocityTextRect;

    HRESULT hr = dwriteFactory->CreateTextFormat(
        L"Arial",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        24.0f,
        L"en-US",
        &m_textFormat
    );
    if (FAILED(hr))
    {
        OutputDebugString("Failed to create DWrite text format\n");
        return;
    }

    hr = m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    if (FAILED(hr))
    {
        OutputDebugString("Failed to set text alignment\n");
        return;
    }

    hr = m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    if (FAILED(hr))
    {
        OutputDebugString("Failed to set paragraph alignment\n");
        return;
    }
}

void GameInfo::Render(
    const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
    const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush
) const
{
    std::wstring time = m_timeText + std::to_wstring(m_time);
    std::wstring velocity = m_velocityText + std::to_wstring(m_velocity);

    d2dDeviceContext->DrawTextA(
        time.c_str(),
        time.length(),
        m_textFormat.Get(),
        m_timeTextRect,
        brush.Get()
    );

    d2dDeviceContext->DrawTextA(
        velocity.c_str(),
        velocity.length(),
        m_textFormat.Get(),
        m_velocityTextRect,
        brush.Get()
    );
}
