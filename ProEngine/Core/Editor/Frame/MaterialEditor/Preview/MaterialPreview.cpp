#include "MaterialPreview.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"
#include "Core/Scene/SceneRenderer.h"
#include <glad/glad.h>

namespace ProEngine {

MaterialPreview::MaterialPreview(const ImVec2& size) : size_(size) {
    camera_controller_ = CreateRef<Camera3DController>(size.x / size.y, Camera3DController::ControlMode::Orbit);
}

void MaterialPreview::OnAttach() {
    FramebufferSpecification spec;
    spec.Width = static_cast<uint32_t>(size_.x);
    spec.Height = static_cast<uint32_t>(size_.y);
    spec.EnableEntityIDAttachment = true;
    framebuffer_ = Framebuffer::Create(spec);
    camera_controller_->OnResize(spec.Width, spec.Height);

    scene_ = CreateRef<Scene>();

    sphere_entity_ = scene_->CreateEntity("PreviewSphere");
    preview_material_ = CreateRef<Material>();
    Ref<Mesh> sphereMesh = Mesh::CreateSphere();
    sphereMesh->SetMaterial(preview_material_);
    RendererComponent rc;
    rc.mesh_ptr = sphereMesh;
    sphere_entity_.AddComponent<RendererComponent>(rc);
}

void MaterialPreview::OnResize(const ImVec2& size) {
    size_ = size;
    framebuffer_->Resize(static_cast<uint32_t>(size_.x), static_cast<uint32_t>(size_.y));
    camera_controller_->OnResize(size_.x, size_.y);
}

void MaterialPreview::OnUpdate(Timestep ts) {
    camera_controller_->OnUpdate(ts);

    framebuffer_->Bind();
    int idClear = -1;
    glClearBufferiv(GL_COLOR, 1, &idClear);
    RenderCommand::Clear();
    Renderer3D::BeginScene(camera_controller_->GetCamera());
    Renderer3D::SetAmbientLight(glm::vec3(1.0f), 10.0);

    scene_->OnUpdate(ts);

    Renderer3D::EndScene();
    framebuffer_->Unbind();
}

void MaterialPreview::OnEvent(Event& e) {
    camera_controller_->OnEvent(e);
}

void MaterialPreview::OnImGuiRender() {
    ImGui::BeginChild("Preview", size_, true,
                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                          ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMouseInputs);
    ImGui::Text("Preview");
    ImVec2 avail = ImGui::GetContentRegionAvail();
    if ((uint32_t)avail.x != framebuffer_->GetSpecification().Width ||
        (uint32_t)avail.y != framebuffer_->GetSpecification().Height) {
        OnResize(ImVec2(avail.x, avail.y));
    }
    ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), avail, ImVec2{0,1}, ImVec2{1,0});
    ImGui::EndChild();
}

} // namespace ProEngine

