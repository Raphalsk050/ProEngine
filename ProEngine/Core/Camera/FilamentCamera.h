#pragma once

#include "Core/Camera/Camera.h"
#include <filament/Engine.h>
#include <filament/Camera.h>
#include <utils/Entity.h>
#include <glm.hpp>

namespace ProEngine {

class FilamentCamera : public Camera {
public:
    FilamentCamera() = default;
    FilamentCamera(filament::Engine* engine,
                   float fov, float aspectRatio,
                   float nearClip, float farClip);
    ~FilamentCamera();

    void SetPerspective(float fov, float aspectRatio,
                        float nearClip, float farClip);
    void SetFov(float fov);
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const { return position_; }
    void SetForwardDirection(const glm::vec3& direction);
    glm::vec3 GetForwardDirection() const { return forward_; }

    filament::Camera* Get() const { return camera_; }
    utils::Entity GetEntity() const { return entity_; }

private:
    filament::Engine* engine_ = nullptr;
    filament::Camera* camera_ = nullptr;
    utils::Entity entity_{};
    glm::vec3 position_{0.0f, 0.0f, 3.0f};
    glm::vec3 forward_{0.0f, 0.0f, -1.0f};
    float fov_ = 45.0f;
    float aspectRatio_ = 800.0f/600.0f;
    float nearClip_ = 0.1f;
    float farClip_ = 1000.0f;
};

} // namespace ProEngine
