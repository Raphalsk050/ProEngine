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
        EntityHandle sphere_entity_;
        EntityHandle cube_entity_;
        EntityHandle cube_entity_2_;
        EntityHandle model_entity_;

        EntityHandle plane_entity_;

        // line
        glm::vec3 line_p1_position_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 line_p2_position_ = {1.0f, 0.0f, 0.0f};
        glm::vec4 line_color_ = {0.0f, 0.0f, 1.0f, 1.0f};

        RendererComponent render_component_0_;
        RendererComponent render_component_1_;
    };
} // namespace ProEngine
