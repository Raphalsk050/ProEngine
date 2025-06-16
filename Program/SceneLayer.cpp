#include "SceneLayer.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"

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
}

void SceneLayer::OnDetach() {
    Layer::OnDetach();
}

void SceneLayer::OnUpdate(Timestep ts) {
    Layer::OnUpdate(ts);
    camera_controller_.OnUpdate(ts);
    time_ += ts;
    RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
    RenderCommand::Clear();
    Renderer3D::BeginScene(camera_controller_.GetCamera());
    Renderer3D::DrawCube(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.8f,0.1f,0.1f,1.0f));
    Renderer3D::EndScene();
}
} // namespace ProEngine
