#pragma once

#include "Core/Camera/FilamentCamera.h"
#include "Core/Timestep.h"
#include "Core/Event/Event.h"
#include "Core/Event/WindowApplicationEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/KeyEvent.h"

namespace ProEngine {

class FilamentCameraController {
public:
    FilamentCameraController(filament::Engine* engine, float aspectRatio);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    void OnResize(float width, float height);

    FilamentCamera& GetCamera() { return m_Camera; }
    const FilamentCamera& GetCamera() const { return m_Camera; }

private:
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnKeyReleased(KeyReleasedEvent& e);

    FilamentCamera m_Camera;
    float movementSpeed_ = 5.0f;
    bool forward_ = false;
    bool backward_ = false;
    bool left_ = false;
    bool right_ = false;
    bool up_ = false;
    bool down_ = false;
};

} // namespace ProEngine
