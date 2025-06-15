#pragma once

// Enable GLM experimental features before including GLM headers
#define GLM_ENABLE_EXPERIMENTAL
#include "Core/Camera/Camera.h"
#include <array>

namespace ProEngine {

// Frustum planes enumeration for clarity
enum class FrustumPlane {
    Left = 0,
    Right,
    Bottom,
    Top,
    Near,
    Far
};

class Camera3D : public Camera {
public:
    Camera3D() = default;
    Camera3D(float fov, float aspectRatio, float nearClip, float farClip);

    void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
    void SetViewport(float width, float height);
    void SetCameraForwardDirection(glm::vec3 direction);
    void SetFov(float fov);

    // Updates the view matrix and frustum planes
    void RecalculateViewMatrix();
    void RecalculateFrustum();

    // Frustum culling methods
    bool PointInFrustum(const glm::vec3& point) const;
    bool SphereInFrustum(const glm::vec3& center, float radius) const;
    bool AABBInFrustum(const glm::vec3& min, const glm::vec3& max) const;


    // Position and orientation control
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const { return camera_pos_; }

    // Direction vectors
    glm::vec3 GetForwardDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetUpDirection() const;

    // Matrices access
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

    // Frustum info
    const std::array<glm::vec4, 6>& GetFrustumPlanes() const { return m_FrustumPlanes; }

    // Camera properties
    float GetNearClip() const { return m_NearClip; }
    float GetFarClip() const { return m_FarClip; }
    float GetAspectRatio() const { return m_AspectRatio; }
    float GetFOV() const { return m_FOV; }

    // Debug functions
    void DebugFrustum() const;
    bool TestSphereAtPosition(const glm::vec3& center, float radius) const;

private:
    glm::vec3 camera_pos_   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up_    = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::vec3 camera_target_ = glm::vec3(0.0f, 0.0f, 0.0f);

    // Camera projection properties
    float m_FOV = 45.0f;
    float m_AspectRatio = 1.778f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;

    // View and projection matrices
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

    // Frustum culling
    std::array<glm::vec4, 6> m_FrustumPlanes; // Left, Right, Bottom, Top, Near, Far

    void ExtractFrustumPlanes(const glm::mat4& viewProj);
};

} // namespace BEngine