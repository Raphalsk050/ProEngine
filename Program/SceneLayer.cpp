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
        plane_entity_ = scene->CreateEntity("plane");

        Ref<Material> mat = CreateRef<Material>();
        mat->SetAlbedoMap(Texture2D::Create("../ProEngine/Assets/Editor/Textures/grid_texture_alpha.png"));
        mat->SetTilingFactor(glm::vec2(100.0f, 100.0f));
        RendererComponent rc;
        auto plane = Mesh::CreatePlane(1.0f,1.0f);
        plane->SetMaterial(mat);
        rc.mesh = MeshType::Model;
        rc.mesh_ptr = plane;
        plane_entity_.AddComponent<RendererComponent>(rc);
        plane_entity_.SetScale(glm::vec3(10000.0f,0.0001f,10000.0f));

        // Create an example entity with renderer component
        // auto* scene = Application::Get().GetActiveScene();
        // sphere_entity_ = scene->CreateEntity("Sphere");
        // cube_entity_ = scene->CreateEntity("Cube");

        // RendererComponent rc = RendererComponent({.mesh = MeshType::Sphere});
        // render_component_0_ = sphere_entity_.AddComponent<RendererComponent>(rc);
        // rc = RendererComponent({.mesh = MeshType::Cube});
        // render_component_1_ = cube_entity_.AddComponent<RendererComponent>(rc);
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
