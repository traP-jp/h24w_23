#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <d2d1_3.h>
#include <wrl/client.h>

#include <string>

class GameInfo
{
public:
    GameInfo() = default;

    void Init(
        const Microsoft::WRL::ComPtr<IDWriteFactory>& dwriteFactory,
        const D2D1_RECT_F& timeTextRect,
        const D2D1_RECT_F& velocityTextRect
    );

    void Render(
        const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
        const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush
    ) const;

private:
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

    D2D1_RECT_F m_timeTextRect;
    D2D1_RECT_F m_velocityTextRect;

    std::wstring m_timeText = L"Time: ";
    std::wstring m_velocityText = L"Velocity: ";

    int m_time = 300;
    float m_velocity = 0.0f;
};

#endif  // GAMEINFO_H
