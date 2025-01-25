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
        const D2D1_RECT_F& velocityTextRect,
        const D2D1_RECT_F& fpsRect,
        const D2D1_RECT_F& leftBulletRect
    );

    void Render(
        const Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& d2dDeviceContext,
        const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush
    ) const;

    void Frame()
    {
        m_time -= 1;
    }

    void SetVelocity(float velocity)
    {
        m_velocity = velocity * 120.0f;
    }

    void SetFPS(float fps)
    {
        m_fps = fps;
    }

    void SetLeftBullet(int leftBullet)
    {
        m_leftBullet = leftBullet;
    }

private:
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

    D2D1_RECT_F m_timeTextRect;
    D2D1_RECT_F m_velocityTextRect;
    D2D1_RECT_F m_fpsRect;
    D2D1_RECT_F m_leftBulletRect;

    std::wstring m_timeText = L"Time Left: ";
    std::wstring m_velocityText = L"Velocity: ";
    std::wstring m_fpsText = L"FPS: ";
    std::wstring m_leftBulletText = L"Bullets: ";

    int m_time = 1200;
    float m_velocity = 0.0f;
    float m_fps = 0.0f;
    int m_leftBullet = 0;
};

#endif  // GAMEINFO_H
