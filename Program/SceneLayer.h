#pragma once
#include "Core/Camera/Camera3DController.h"
#include "Core/Layer/Layer.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Scene/EntityHandle.h"
#include "Core/Scene/Components.h"

namespace ProEngine {
    class SceneLayer : public Layer {
    public:
        SceneLayer();
        void OnAttach() override;
        void OnUpdate(Timestep ts) override;
        void OnDetach() override;
        void OnEvent(Event& e) override;
        void OnImGuiRender() override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool camera_movement_enabled_ = false;
        Ref<Framebuffer> framebuffer_;
        glm::vec2 viewport_size_ = {0.0f, 0.0f};
        std::unordered_map<KeyCode, bool> last_key_state_map_;
        Camera3DController camera_controller_ = Camera3DController(1.0f);
        float time_ = 0.0f;
        EntityHandle cube_entity_0_;
        EntityHandle cube_entity_1_;
        EntityHandle cube_entity_2_;
        EntityHandle cube_entity_3_;

        // line
        glm::vec3 line_p1_position_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 line_p2_position_ = {1.0f, 0.0f, 0.0f};
        glm::vec4 line_color_ = {0.0f, 0.0f, 1.0f, 1.0f};

        RendererComponent render_command_1_;
        RendererComponent render_command_2_;
    };
} // namespace ProEngine
