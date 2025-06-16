#include "SceneRenderer.h"

namespace ProEngine {

void SceneRenderer::RenderScene(Scene* scene)
{
    auto& registry = scene->GetRegistry();
    auto view = registry.view<TransformComponent, RendererComponent>();
    for (auto entity : view)
    {
        auto& transform = view.get<TransformComponent>(entity);
        auto& renderer = view.get<RendererComponent>(entity);

        glm::mat4 worldTransform = scene->GetWorldMatrix(entity);
        int entityID = (int)entt::to_integral(entity);

        if (renderer.model)
        {
            ModelRendererComponent temp;
            temp.model = renderer.model;
            Renderer3D::DrawModel(worldTransform, temp, entityID);
        }
        else if (renderer.mesh_ptr)
        {
            Renderer3D::DrawMesh(worldTransform, renderer.mesh_ptr, renderer.color, entityID);
        }
        else
        {
            switch (renderer.mesh)
            {
            case MeshType::Cube:
                Renderer3D::DrawCube(worldTransform, renderer.color, entityID);
                break;
            case MeshType::Sphere:
                Renderer3D::DrawSphere(worldTransform, renderer.color, entityID);
                break;
            default:
                Renderer3D::DrawBox(worldTransform, renderer.color, entityID);
                break;
            }
        }
    }
}

} // namespace ProEngine
