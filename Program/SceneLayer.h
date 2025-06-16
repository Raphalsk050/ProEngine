#pragma once
#include "Core/Camera/Camera3DController.h"
#include "Core/Layer/Layer.h"
#include "Core/Renderer/Mesh.h"

namespace ProEngine {
    class SceneLayer : public Layer {
    public:
        SceneLayer();
        void OnAttach() override;
        void OnUpdate(Timestep ts) override;
        void OnDetach() override;
    private:
        Camera3DController camera_controller_ = Camera3DController(1.0f);
        float time_ = 0.0f;
    };
} // namespace ProEngine
