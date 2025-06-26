#pragma once
#include "Core/Renderer/Framebuffer.h"
#include "Core/Camera/Camera3DController.h"
#include "Core/Renderer/Material.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/EntityHandle.h"
#include "Core/Timestep.h"
#include "Core/Event/Event.h"
#include "imgui.h"

namespace ProEngine {

class MaterialPreview {
public:
    explicit MaterialPreview(const ImVec2& size);

    void OnAttach();
    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    void OnResize(const ImVec2& size);
    void OnImGuiRender();

    Ref<Material> GetMaterial() const { return preview_material_; }
    Ref<Framebuffer> GetFramebuffer() const { return framebuffer_; }

private:
    ImVec2 size_;
    Ref<Framebuffer> framebuffer_;
    Ref<Scene> scene_;
    Ref<Camera3DController> camera_controller_;
    EntityHandle sphere_entity_;
    Ref<Material> preview_material_;
};

} // namespace ProEngine

