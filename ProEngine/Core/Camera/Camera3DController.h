#pragma once

#include "Core/Camera/Camera3D.h"
#include "Core/TimeStep.h"
#include "Core/Event/WindowApplicationEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/KeyEvent.h"

namespace ProEngine {

class Camera3DController
{
public:
    enum class ControlMode {
        Fly,     // Free-flying camera
        Orbit,   // Orbiting around a target
        FirstPerson // First person camera (no roll)
    };
    
    Camera3DController(float aspectRatio, ControlMode mode = ControlMode::Fly);
    
    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    void OnResize(float width, float height);
    
    // Camera access
    Camera3D& GetCamera() { return m_Camera; }
    const Camera3D& GetCamera() const { return m_Camera; }
    
    // Camera settings
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const { return m_Camera.GetPosition(); }
    
    void SetRotation(const glm::vec3& rotation);

    void SetFocalPoint(const glm::vec3& focalPoint);

    void SetDistance(float distance) { m_Distance = distance; }
    float GetDistance() const { return m_Distance; }
    
    // Control mode
    void SetControlMode(ControlMode mode) { m_ControlMode = mode; }
    ControlMode GetControlMode() const { return m_ControlMode; }
    
    // Movement speed settings
    void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
    float GetMovementSpeed() const { return m_MovementSpeed; }
    
    void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }
    float GetRotationSpeed() const { return m_RotationSpeed; }
    
    void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; }
    float GetZoomSpeed() const { return m_ZoomSpeed; }
    
    // Enable/disable mouse control
    void SetMouseControl(bool enabled) { m_MouseControlEnabled = enabled; }
    bool IsMouseControlEnabled() const { return m_MouseControlEnabled; }
    
private:
    // Event handlers
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnKeyReleased(KeyReleasedEvent& e);

    // Update methods for different control modes
    void UpdateFlyMode(Timestep ts);
    void UpdateOrbitMode(Timestep ts);
    void UpdateFirstPersonMode(Timestep ts);
    
    // Utility methods
    void UpdateCameraPositionFromOrbit();
    
private:
    // The controlled camera
    Camera3D m_Camera;
    
    // Control settings
    ControlMode m_ControlMode = ControlMode::Orbit;
    bool m_MouseControlEnabled = true;
    bool m_MouseButtonPressed = false;
    float mouse_sensitivity_ = 0.1f;
    
    // Camera parameters
    float m_AspectRatio = 1.33f;
    float m_Distance = 10.0f;      // Distance from focal point in orbit mode
    float m_Pitch = 0.0f;          // In degrees
    float m_Yaw = 0.0f;            // In degrees
    float m_Roll = 0.0f;           // In degrees
    
    // Mouse state
    glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
    bool m_FirstMouse = true;
    
    // Movement settings
    float m_MovementSpeed = 5.0f;
    float m_RotationSpeed = 0.01f;  // Degrees per pixel for mouse movement
    float m_ZoomSpeed = 1.0f;
    
    // Key states for smooth movement
    bool m_Forward = false;
    bool m_Backward = false;
    bool m_Left = false;
    bool m_Right = false;
    bool m_Up = false;
    bool m_Down = false;
};

} // namespace BEngine