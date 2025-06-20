#include "Core/Camera/FilamentCameraController.h"
#include "PEPCH.h"

namespace ProEngine {

FilamentCameraController::FilamentCameraController(filament::Engine* engine,
                                                   float aspectRatio)
    : m_Camera( new FilamentCamera(engine, 45.0f, aspectRatio, 0.1f, 1000.0f)) {}

void FilamentCameraController::OnUpdate(Timestep ts) {
    glm::vec3 forward = m_Camera->GetForwardDirection();
    glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 position = m_Camera->GetPosition();
    float velocity = movementSpeed_ * ts;

    if (forward_) position += forward * velocity;
    if (backward_) position -= forward * velocity;
    if (right_) position += right * velocity;
    if (left_) position -= right * velocity;
    if (up_) position.y += velocity;
    if (down_) position.y -= velocity;

    m_Camera->SetPosition(position);
}

void FilamentCameraController::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(FilamentCameraController::OnKeyPressed));
    dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(FilamentCameraController::OnKeyReleased));
}

bool FilamentCameraController::OnKeyPressed(KeyPressedEvent& e) {
    switch (e.GetKeyCode()) {
    case Key::W: forward_ = true; break;
    case Key::S: backward_ = true; break;
    case Key::A: left_ = true; break;
    case Key::D: right_ = true; break;
    case Key::Q: up_ = true; break;
    case Key::E: down_ = true; break;
    }
    return false;
}

bool FilamentCameraController::OnKeyReleased(KeyReleasedEvent& e) {
    switch (e.GetKeyCode()) {
    case Key::W: forward_ = false; break;
    case Key::S: backward_ = false; break;
    case Key::A: left_ = false; break;
    case Key::D: right_ = false; break;
    case Key::Q: up_ = false; break;
    case Key::E: down_ = false; break;
    }
    return false;
}

void FilamentCameraController::OnResize(float width, float height) {
    m_Camera->SetPerspective(45.0f, width/height, 0.1f, 1000.0f);
}

} // namespace ProEngine
