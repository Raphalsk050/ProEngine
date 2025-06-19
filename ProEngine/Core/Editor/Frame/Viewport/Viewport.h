#pragma once
#include "Core/Camera/Camera3DController.h"
#include "Core/Editor/Frame/Hierarchy/HierarchyInspector.h"
#include "Core/Renderer/Framebuffer.h"


namespace ProEngine
{
    class Viewport : public Layer
    {
    public:
        Viewport(HierarchyInspector* hierarchy_inspector);
        ~Viewport() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
        void Open();
        void Close();
        void ToggleWindow();
        Camera3D* GetCamera() { return &camera_controller_.GetCamera(); }
        ImVec2 GetViewportSize() { return viewport_size_; }
        ImVec2 GetViewportLocation() { return viewport_location_; }

    private:
        float time_ = 0.0f;
        bool opened_ = true;
        bool camera_movement_enabled_ = false;
        ImVec2 viewport_location_ = ImVec2(0, 0);
        ImVec2 viewport_size_ = ImVec2(0, 0);
        Ref<Framebuffer> framebuffer_;
        HierarchyInspector* hierarchy_inspector_;
        std::unordered_map<KeyCode, bool> last_key_state_map_;
        glm::vec2 window_size_ = {0.0f, 0.0f};
        Camera3DController camera_controller_ = Camera3DController(1.0f);
        EntityHandle plane_entity_;

    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        void InitializeGrid();
    };
} // ProEngine
