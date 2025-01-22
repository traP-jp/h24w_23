#ifndef CARASSIUS_CAMERA_H
#define CARASSIUS_CAMERA_H

#include <AquaEngine.h>

#include <memory>

class Camera
{
public:
    Camera(const RECT& rc, bool is_player1)
        : m_camera(nullptr), m_isPlayer1(is_player1), m_rc(rc)
    {
    }
    void Init();
    void Render(AquaEngine::Command& command) const;
    void Accel(DirectX::XMVECTOR delta);
    void Decel(DirectX::XMVECTOR delta);
    void RotRight(const DirectX::XMMATRIX& transform);
    void RotLeft(const DirectX::XMMATRIX& transform);
    void RotUp(const DirectX::XMMATRIX& transform);
    void RotDown(const DirectX::XMMATRIX& transform);
    void Move(DirectX::XMVECTOR dr);

    [[nodiscard]] std::shared_ptr<AquaEngine::Camera> GetCamera() const
    {
        return m_camera;
    }

private:
    std::shared_ptr<AquaEngine::Camera> m_camera;
    bool m_isPlayer1;
    RECT m_rc;

    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_POSITION
        = {1.0f, 0.0f, 0.0f};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_POSITION
        = {-1.0f, 0.0f, 0.0f};

    static constexpr float DEFAULT_CAMERA_Z_DISTANCE = -1.5f;
    static constexpr DirectX::XMFLOAT3 PLAYER1_DEFAULT_CAMERA
        = {1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE * 2};
    static constexpr DirectX::XMFLOAT3 PLAYER2_DEFAULT_CAMERA
        = {-1.0f, -DEFAULT_CAMERA_Z_DISTANCE, DEFAULT_CAMERA_Z_DISTANCE * 2};

    // static constexpr std::array<float, 10> START_ACCELERATION
    //     = {0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.072, 0.064, 0.056, 0.048};
    static constexpr float ACCELERATION = 0.04f;
    static constexpr float PLAYER_MAX_DISTANCE = 5.0f;
};


#endif //CARASSIUS_CAMERA_H
