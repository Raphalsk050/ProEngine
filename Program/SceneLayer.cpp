#include "SceneLayer.h"

#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/Renderer3D.h"

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
    }

    void SceneLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
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
