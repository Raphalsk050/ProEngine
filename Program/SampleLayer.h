#pragma once
#include "Core/Camera/Camera3DController.h"
#include "Core/Editor/SimpleAnimatedPopup.h"
#include "Core/Layer/Layer.h"
#include "Core/Renderer/Mesh.h"

namespace ProEngine
{
    class Framebuffer;

    class SampleLayer : public Layer
    {
    public:
        SampleLayer();
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

    private:
        char input_text_[256] = "";
        Ref<Mesh> mesh_;
        Ref<Framebuffer> framebuffer_;
        glm::vec2 viewport_size_ = {0.0f, 0.0f};
        float time_ = 0.0f;
        Camera3DController camera_controller_ = Camera3DController(1);
    };
} // ProEngine
