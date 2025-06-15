#include "Core/Camera/Camera3D.h"
#include "PEPCH.h"

#include <gtc/matrix_transform.hpp>

// The header needs to be included here after defining GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

#include "Core/Application/Application.h"
#include "gtc/type_ptr.hpp"

namespace ProEngine {

Camera3D::Camera3D(float fov, float aspectRatio, float nearClip, float farClip)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
      Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{
    RecalculateViewMatrix();
    RecalculateFrustum();

    SetViewport(Application::Get().GetWindow().GetWidth(),Application::Get().GetWindow().GetHeight());
}

void Camera3D::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
{
    m_FOV = fov;
    m_AspectRatio = aspectRatio;
    m_NearClip = nearClip;
    m_FarClip = farClip;
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    RecalculateFrustum();
}

void Camera3D::SetViewport(float width, float height)
{
    m_AspectRatio = width / height;
    PENGINE_CORE_INFO("Viewport ({},{})", width, height);
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    RecalculateFrustum();
    RecalculateViewMatrix();
}

void Camera3D::SetCameraForwardDirection(glm::vec3 direction)
{
    camera_front_ = direction;
}

void Camera3D::SetFov(float fov)
{
    SetPerspective(fov, m_AspectRatio, m_NearClip, m_FarClip);
}

void Camera3D::RecalculateViewMatrix()
{
    PENGINE_PROFILE_FUNCTION();

    m_ViewMatrix = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);

    RecalculateFrustum();
}

void Camera3D::RecalculateFrustum()
{
    PENGINE_PROFILE_FUNCTION();

    // Get the view-projection matrix
    glm::mat4 viewProj = m_Projection * m_ViewMatrix;

    ExtractFrustumPlanes(viewProj);
}

void Camera3D::ExtractFrustumPlanes(const glm::mat4& viewProj)
{
    // Extrair planos usando método de Gribb/Hartmann
    // Mais robusto que o método anterior

    // Left plane: add 4th column to 1st column
    m_FrustumPlanes[0] = glm::vec4(
        viewProj[0][3] + viewProj[0][0],
        viewProj[1][3] + viewProj[1][0],
        viewProj[2][3] + viewProj[2][0],
        viewProj[3][3] + viewProj[3][0]
    );

    // Right plane: subtract 1st column from 4th column
    m_FrustumPlanes[1] = glm::vec4(
        viewProj[0][3] - viewProj[0][0],
        viewProj[1][3] - viewProj[1][0],
        viewProj[2][3] - viewProj[2][0],
        viewProj[3][3] - viewProj[3][0]
    );

    // Bottom plane: add 4th column to 2nd column
    m_FrustumPlanes[2] = glm::vec4(
        viewProj[0][3] + viewProj[0][1],
        viewProj[1][3] + viewProj[1][1],
        viewProj[2][3] + viewProj[2][1],
        viewProj[3][3] + viewProj[3][1]
    );

    // Top plane: subtract 2nd column from 4th column
    m_FrustumPlanes[3] = glm::vec4(
        viewProj[0][3] - viewProj[0][1],
        viewProj[1][3] - viewProj[1][1],
        viewProj[2][3] - viewProj[2][1],
        viewProj[3][3] - viewProj[3][1]
    );

    // Near plane: add 4th column to 3rd column
    m_FrustumPlanes[4] = glm::vec4(
        viewProj[0][3] + viewProj[0][2],
        viewProj[1][3] + viewProj[1][2],
        viewProj[2][3] + viewProj[2][2],
        viewProj[3][3] + viewProj[3][2]
    );

    // Far plane: subtract 3rd column from 4th column
    m_FrustumPlanes[5] = glm::vec4(
        viewProj[0][3] - viewProj[0][2],
        viewProj[1][3] - viewProj[1][2],
        viewProj[2][3] - viewProj[2][2],
        viewProj[3][3] - viewProj[3][2]
    );

    for (auto& plane : m_FrustumPlanes)
    {
        float length = glm::length(glm::vec3(plane));
        if (length > 0.0001f) { // Evitar divisão por zero
            plane /= length;
        }
    }
}

    bool Camera3D::PointInFrustum(const glm::vec3& point) const
{
    // Check if the point is inside all frustum planes
    for (const auto& plane : m_FrustumPlanes)
    {
        float distance = glm::dot(glm::vec3(plane), point) + plane.w;
        if (distance < 0.0f)
            return false;
    }
    return true;
}

bool Camera3D::SphereInFrustum(const glm::vec3& center, float radius) const
{
    // Check if the sphere is completely outside any frustum plane
    for (const auto& plane : m_FrustumPlanes)
    {
        float distance = glm::dot(glm::vec3(plane), center) + plane.w;
        if (distance < -radius)
            return false;
    }

    return true;
}

bool Camera3D::AABBInFrustum(const glm::vec3& min, const glm::vec3& max) const
{
    for (const auto& plane : m_FrustumPlanes)
    {
        glm::vec3 normal = glm::vec3(plane);

        // Find the positive vertex (P-vertex) - farthest point in direction of normal
        glm::vec3 positiveVertex;
        positiveVertex.x = (normal.x >= 0.0f) ? max.x : min.x;
        positiveVertex.y = (normal.y >= 0.0f) ? max.y : min.y;
        positiveVertex.z = (normal.z >= 0.0f) ? max.z : min.z;

        // If the positive vertex is outside, the entire AABB is outside
        if (glm::dot(normal, positiveVertex) + plane.w < 0.0f)
            return false;
    }
    return true;
}

void Camera3D::DebugFrustum() const
{
#ifdef FENGINE_DEBUG_FRUSTUM
    FENGINE_CORE_INFO("=== FRUSTUM DEBUG ===");
    FENGINE_CORE_INFO("Camera Position: ({:.2f}, {:.2f}, {:.2f})",
                     camera_pos_.x, camera_pos_.y, camera_pos_.z);
    FENGINE_CORE_INFO("Camera Front: ({:.2f}, {:.2f}, {:.2f})",
                     camera_front_.x, camera_front_.y, camera_front_.z);

    const char* planeNames[] = {"Left", "Right", "Bottom", "Top", "Near", "Far"};
    for (int i = 0; i < 6; i++) {
        const auto& plane = m_FrustumPlanes[i];
        FENGINE_CORE_INFO("{} Plane: ({:.3f}, {:.3f}, {:.3f}, {:.3f})",
                         planeNames[i], plane.x, plane.y, plane.z, plane.w);
    }
#endif
}

bool Camera3D::TestSphereAtPosition(const glm::vec3& center, float radius) const
{
    bool result = SphereInFrustum(center, radius);
    PENGINE_CORE_INFO("Testing sphere at ({:.2f}, {:.2f}, {:.2f}) radius {:.2f}: {}",
                     center.x, center.y, center.z, radius, result ? "VISIBLE" : "CULLED");

    // Debug: mostrar distância para cada plano
    for (int i = 0; i < 6; i++) {
        const auto& plane = m_FrustumPlanes[i];
        float distance = glm::dot(glm::vec3(plane), center) + plane.w;
        const char* planeNames[] = {"Left", "Right", "Bottom", "Top", "Near", "Far"};
        PENGINE_CORE_INFO("  {} plane distance: {:.2f} (radius: {:.2f})",
                         planeNames[i], distance, radius);
    }

    return result;
}

void Camera3D::SetPosition(const glm::vec3& position)
{
    camera_pos_ = position;
    RecalculateViewMatrix();
}

glm::vec3 Camera3D::GetForwardDirection() const
{
    return camera_front_;
}

glm::vec3 Camera3D::GetRightDirection() const
{
    return glm::cross(camera_front_,camera_up_);
}

glm::vec3 Camera3D::GetUpDirection() const
{
    return camera_up_;
}

} // namespace BEngine