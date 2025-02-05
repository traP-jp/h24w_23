#ifndef CARASSIUS_CAMERA_H
#define CARASSIUS_CAMERA_H

#include <AquaEngine.h>

#include <memory>

class Camera
{
public:
    Camera(const RECT &rc, bool is_player1) : m_camera(nullptr), m_isPlayer1(is_player1), m_rc(rc)
    {
    }

    void Init();

    void InitBullet();

    void InitSideUI();

    void InitEye();

    void Render(AquaEngine::Command &command) const;

    void RenderBullet(AquaEngine::Command &command) const;

    void RenderSideUI(AquaEngine::Command &command) const;

    void RenderEye(AquaEngine::Command &command) const;

    void StartFrame(DirectX::XMVECTOR direction, DirectX::XMVECTOR up) const;

    void EndFrame(DirectX::XMVECTOR direction, DirectX::XMVECTOR up);

    void Accel(int accelFrame);

    void Decel(DirectX::XMVECTOR delta);

    void Rot(const DirectX::XMMATRIX &transform);

    void Move(DirectX::XMVECTOR dr);

    [[nodiscard]] std::shared_ptr<AquaEngine::Camera> GetCamera() const
    {
        return m_camera;
    }

private:
    std::shared_ptr<AquaEngine::Camera> m_camera;
    bool m_isPlayer1;
    RECT m_rc;
    DirectX::XMVECTOR m_delta;  // distance from default camera position
    float m_deltaDistance = 0.0f;

    // GameView.hも一緒に変える
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_POSITION = {1.0f, 0.0f, 0.0f};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_POSITION = {-1.0f, 0.0f, 0.0f};

    static constexpr float DEFAULT_CAMERA_Z_DISTANCE = -4.2f;
    static constexpr float CAMERA_ASPECT = 1.2f;
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_CAMERA
        = {1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE *CAMERA_ASPECT};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_CAMERA
        = {-1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE *CAMERA_ASPECT};

    // static constexpr std::array<float, 10> START_ACCELERATION
    //     = {0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.072, 0.064, 0.056, 0.048};
    static constexpr float ACCELERATION = 0.04f;
    static constexpr float PLAYER_MAX_DISTANCE = 5.0f;

    static constexpr float CAMERA_ACCEL_DISTANCE = 4.0f;

    // player mo henshu
    static constexpr int ACCEL_FIRST_FRAME = 3;
    static constexpr int ACCEL_SECOND_FRAME = 15;
    static constexpr int ACCEL_THIRD_FRAME = 21;
};

#endif  // CARASSIUS_CAMERA_H
