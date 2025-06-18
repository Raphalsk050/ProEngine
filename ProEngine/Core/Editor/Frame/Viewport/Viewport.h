#pragma once
#include "Core/Camera/Camera3DController.h"
#include "Core/Renderer/Framebuffer.h"


namespace ProEngine
{
    class Viewport : public Layer
    {
    public:
        Viewport();
        ~Viewport() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
        void Open();
        void Close();
        void ToggleWindow();

    private:
        float time_ = 0.0f;
        bool opened_ = true;
        bool camera_movement_enabled_ = false;
        Ref<Framebuffer> framebuffer_;
        std::unordered_map<KeyCode, bool> last_key_state_map_;
        glm::vec2 viewport_size_ = {0.0f, 0.0f};
        Camera3DController camera_controller_ = Camera3DController(1.0f);

    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
    };
} // ProEngine
