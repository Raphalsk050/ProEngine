#include "SceneLayer.h"
#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/Renderer3D.h"
#include <filament/FilamentAPI.h>
#include <filament/Engine.h>
#include "ThirdParty/filament/stub/MinimalScene.h"
#include <glad/glad.h>

namespace ProEngine
{
    SceneLayer::SceneLayer() : Layer("SceneLayer")
    {
    }

    void SceneLayer::OnAttach()
    {
        Layer::OnAttach();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Initialize Filament stub engine and framebuffer
        filament_engine_ = filament::Engine::create();

        FramebufferSpecification spec;
        spec.Width = Application::Get().GetWindow().GetWidth();
        spec.Height = Application::Get().GetWindow().GetHeight();
        framebuffer_ = Framebuffer::Create(spec);
        filament_pixels_.resize(spec.Width * spec.Height);

        auto* scene = Application::Get().GetActiveScene();

        RendererComponent rc;
        // Create an example entity with renderer component
        sphere_entity_ = scene->CreateEntity("Sphere");
        cube_entity_ = scene->CreateEntity("Cube");
        model_entity_ = scene->CreateEntity("Model");
        auto model = Model::Load("../ProEngine/Assets/Models/monkey.fbx");
        rc.model = model;
        rc = RendererComponent({.mesh = MeshType::Model, .model = model});
        model_entity_.AddComponent<RendererComponent>(rc);
        rc = RendererComponent({.mesh = MeshType::Sphere});
        render_component_0_ = sphere_entity_.AddComponent<RendererComponent>(rc);
        rc = RendererComponent({.mesh = MeshType::Cube});
        render_component_1_ = cube_entity_.AddComponent<RendererComponent>(rc);
    }

    void SceneLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);

        auto spec = framebuffer_->GetSpecification();
        filament::RenderMinimalScene(filament_pixels_.data(), spec.Width, spec.Height);

        framebuffer_->Bind();
        glBindTexture(GL_TEXTURE_2D, framebuffer_->GetColorAttachmentRendererID());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, spec.Width, spec.Height, GL_RGBA, GL_UNSIGNED_BYTE, filament_pixels_.data());
        framebuffer_->Unbind();
    }

    void SceneLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        if (ImGui::Begin("Filament View"))
        {
            ImVec2 size = ImGui::GetContentRegionAvail();
            auto spec = framebuffer_->GetSpecification();
            if (size.x > 0 && size.y > 0 && ((uint32_t)size.x != spec.Width || (uint32_t)size.y != spec.Height))
            {
                framebuffer_->Resize((uint32_t)size.x, (uint32_t)size.y);
                filament_pixels_.resize((uint32_t)size.x * (uint32_t)size.y);
            }
            ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), size, ImVec2{0,1}, ImVec2{1,0});
        }
        ImGui::End();
    }

    void SceneLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void SceneLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
    }
} // namespace ProEngine
