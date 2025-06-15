#include "Core/Camera/Camera3DController.h"
#include "PEPCH.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseCodes.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

namespace ProEngine
{
    Camera3DController::Camera3DController(float aspectRatio, ControlMode mode)
        : m_AspectRatio(aspectRatio), m_ControlMode(mode)
    {
        m_Camera = Camera3D(60.0f, aspectRatio, 0.1f, 1000.0f);
        m_Camera.SetPosition({0.0f, 2.0f, 5.0f});
    }

    void Camera3DController::OnUpdate(Timestep ts)
    {
        PENGINE_PROFILE_FUNCTION();

        // Update based on control mode
        switch (m_ControlMode)
        {
        case ControlMode::Fly:
            UpdateFlyMode(ts);
            break;
        case ControlMode::Orbit:
            UpdateOrbitMode(ts);
            break;
        case ControlMode::FirstPerson:
            UpdateFirstPersonMode(ts);
            break;
        }
    }

    void Camera3DController::UpdateFlyMode(Timestep ts)
    {
        // Get current camera orientation vectors
        glm::vec3 forward = m_Camera.GetForwardDirection();
        glm::vec3 right = m_Camera.GetRightDirection();
        glm::vec3 up = m_Camera.GetUpDirection();

        // Current position
        glm::vec3 position = m_Camera.GetPosition();
        float velocity = m_MovementSpeed * ts;

        // Check input state (from OnKeyPressed events)
        if (m_Forward)
            position += forward * velocity;
        if (m_Backward)
            position -= forward * velocity;
        if (m_Right)
            position += right * velocity;
        if (m_Left)
            position -= right * velocity;
        if (m_Up)
            position += up * velocity;
        if (m_Down)
            position -= up * velocity;

        // Update camera position
        m_Camera.SetPosition(position);
    }

    void Camera3DController::UpdateOrbitMode(Timestep ts)
    {
        // In orbit mode, we rotate around a focal point
        UpdateCameraPositionFromOrbit();
    }

    void Camera3DController::UpdateFirstPersonMode(Timestep ts)
    {
        // Similar to fly mode but with restricted pitch and no roll
        glm::vec3 forward = m_Camera.GetForwardDirection();
        glm::vec3 right = m_Camera.GetRightDirection();

        // Remove vertical component for forward movement (stays at same height)
        forward.y = 0.0f;
        forward = glm::normalize(forward);

        // Current position
        glm::vec3 position = m_Camera.GetPosition();
        float velocity = m_MovementSpeed * ts;

        // Check input state
        if (m_Forward)
            position += forward * velocity;
        if (m_Backward)
            position -= forward * velocity;
        if (m_Right)
            position += right * velocity;
        if (m_Left)
            position -= right * velocity;
        if (m_Up)
            position.y += velocity;
        if (m_Down)
            position.y -= velocity;

        // Update camera position
        m_Camera.SetPosition(position);
    }

    void Camera3DController::UpdateCameraPositionFromOrbit()
    {
        // float pitch = glm::radians(m_Pitch);
        // float yaw = glm::radians(m_Yaw);
        //
        // // Convert spherical coordinates to Cartesian
        // float x = m_Distance * std::cos(pitch) * std::sin(yaw);
        // float y = m_Distance * std::sin(pitch);
        // float z = m_Distance * std::cos(pitch) * std::cos(yaw);
        //
        // glm::vec3 focalPoint = m_Camera.GetFocalPoint();
        // glm::vec3 position = focalPoint + glm::vec3(x, y, z);
        //
        // // Compute rotation to look at focal point
        // glm::vec3 direction = glm::normalize(focalPoint - position);
        // float pitchAngle = glm::asin(direction.y);
        // float yawAngle = glm::atan(direction.x, direction.z);
        //
        // m_Camera.SetPosition(position);
        // m_Camera.SetRotation({pitchAngle, yawAngle, 0.0f});
    }

    void Camera3DController::OnEvent(Event& e)
    {
        PENGINE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseMovedEvent>(PENGINE_BIND_EVENT_FN(Camera3DController::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(PENGINE_BIND_EVENT_FN(Camera3DController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(PENGINE_BIND_EVENT_FN(Camera3DController::OnWindowResized));
        dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(Camera3DController::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(Camera3DController::OnKeyReleased));
    }

    bool Camera3DController::OnMouseMoved(MouseMovedEvent& e)
    {
        if (m_FirstMouse)
        {
            m_LastMousePosition.x = e.GetX();
            m_LastMousePosition.y = e.GetY();
            m_FirstMouse = false;
        }

        float xoffset = e.GetX() - m_LastMousePosition.x;
        float yoffset = m_LastMousePosition.y - e.GetY();
        m_LastMousePosition.x = e.GetX();
        m_LastMousePosition.y = e.GetY();

        xoffset *= mouse_sensitivity_;
        yoffset *= mouse_sensitivity_;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        direction.y = sin(glm::radians(m_Pitch));
        direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Camera.SetCameraForwardDirection(glm::normalize(direction));

        return true;
    }

    bool Camera3DController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        float zoomAmount = e.GetYOffset() * m_ZoomSpeed;

        if (m_ControlMode == ControlMode::Orbit)
        {
            // In orbit mode, adjust the distance
            m_Distance = std::max(1.0f, m_Distance - zoomAmount);
            UpdateCameraPositionFromOrbit();
        }
        else
        {
            // In other modes, zoom changes the movement speed
            m_MovementSpeed = std::max(1.0f, m_MovementSpeed + zoomAmount);
        }

        return false;
    }

    bool Camera3DController::OnWindowResized(WindowResizeEvent& e)
    {
        OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        return true;
    }

    bool Camera3DController::OnKeyPressed(KeyPressedEvent& e)
    {
        bool pressed = (e.IsRepeat() || !e.IsRepeat()); // Include repeats or not
        //FENGINE_CORE_INFO("Key {} state is: {}",e.GetKeyCode(),pressed);
        // Update movement state based on keys
        switch (e.GetKeyCode())
        {
        case Key::W:
            m_Forward = true;
            break;
        case Key::A:
            m_Left = true;
            break;
        case Key::S:
            m_Backward = true;
            break;
        case Key::D:
            m_Right = true;
            break;
        case Key::Q:
            m_Up = true;
            break;
        case Key::E:
            m_Down = true;
            break;
        }

        return false;
    }

    bool Camera3DController::OnKeyReleased(KeyReleasedEvent& e)
    {
        switch (e.GetKeyCode())
        {
        case Key::W:
            m_Forward = false;
            break;
        case Key::A:
            m_Left = false;
            break;
        case Key::S:
            m_Backward = false;
            break;
        case Key::D:
            m_Right = false;
            break;
        case Key::Q:
            m_Up = false;
            break;
        case Key::E:
            m_Down = false;
            break;
        }

        return false;
    }

    void Camera3DController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetViewport(width, height);
        m_Camera.SetPerspective(90.0f, m_AspectRatio, 0.1f, 1000.0f);
    }

    void Camera3DController::SetPosition(const glm::vec3& position)
    {
        m_Camera.SetPosition(position);
    }

    void Camera3DController::SetRotation(const glm::vec3& rotation)
    {
        // Convert from degrees to radians for internal representation
        glm::vec3 radians = glm::vec3(
            glm::radians(rotation.x),
            glm::radians(rotation.y),
            glm::radians(rotation.z)
        );

        //m_Camera.SetRotation(radians);

        // Update derived pitch and yaw for orbit mode
        m_Pitch = rotation.x;
        m_Yaw = rotation.y;
        m_Roll = rotation.z;
    }

    void Camera3DController::SetFocalPoint(const glm::vec3& focalPoint)
    {
        // m_Camera.SetFocalPoint(focalPoint);
        //
        // if (m_ControlMode == ControlMode::Orbit)
        // {
        //     // Update camera position to maintain orbit around new focal point
        //     UpdateCameraPositionFromOrbit();
        // }
    }
} // namespace BEngine
