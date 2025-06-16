#include "SceneLayer.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"
#include "imgui.h"

namespace ProEngine {
SceneLayer::SceneLayer() : Layer("SceneLayer") {
    auto& window = Application::Get().GetWindow();
    camera_controller_ = Camera3DController(window.GetWidth() / (float)window.GetHeight());
}

void SceneLayer::OnAttach() {
    Layer::OnAttach();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    camera_controller_.SetPosition({0.0f, 0.0f, 3.0f});
    camera_controller_.SetRotation({0.0f, 0.0f, 0.0f});
    FramebufferSpecification spec;
    spec.Width = Application::Get().GetWindow().GetWidth();
    spec.Height = Application::Get().GetWindow().GetHeight();
    framebuffer_ = Framebuffer::Create(spec);
    viewport_size_ = {(float)spec.Width, (float)spec.Height};
    camera_controller_.OnResize(spec.Width, spec.Height);
}

void SceneLayer::OnDetach() {
    Layer::OnDetach();
}

void SceneLayer::OnUpdate(Timestep ts) {
    Layer::OnUpdate(ts);
    camera_controller_.OnUpdate(ts);
    time_ += ts;
    framebuffer_->Bind();
    RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    RenderCommand::Clear();
    Renderer3D::BeginScene(camera_controller_.GetCamera());
    Renderer3D::DrawCube(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.8f,0.1f,0.1f,1.0f));
    Renderer3D::EndScene();
    framebuffer_->Unbind();
}

void SceneLayer::OnImGuiRender() {
    Layer::OnImGuiRender();
    ImGui::Begin("Viewport");
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (viewport_size_.x != size.x || viewport_size_.y != size.y) {
        framebuffer_->Resize((uint32_t)size.x, (uint32_t)size.y);
        viewport_size_ = {size.x, size.y};
        camera_controller_.OnResize((uint32_t)size.x, (uint32_t)size.y);
    }
    ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), size, ImVec2(0,1), ImVec2(1,0));
    ImGui::End();
}
} // namespace ProEngine
