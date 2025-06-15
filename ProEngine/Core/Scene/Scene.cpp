#include "Core/Scene/Scene.h"
#include "Core/Scene/EntityHandle.h"
#include "Components.h"


namespace ProEngine
{
    EntityHandle Scene::CreateEntity(const std::string& name)
    {
        entt::entity e = registry_.create();
        EntityHandle handle{e, this};
        registry_.emplace<TransformComponent>(e);
        registry_.emplace<TagComponent>(e, TagComponent{name});
        return handle;
    }

    void Scene::OnUpdate(Timestep ts)
    {
    }

    void Scene::LateUpdate(Timestep ts)
    {
    }

    void Scene::OnEvent(Event& e)
    {
    }

    std::vector<EntityHandle*> Scene::GetAllEntities()
    {
        std::vector<EntityHandle*> handles = std::vector<EntityHandle*>();
        auto view = GetRegistry().view<TransformComponent, TagComponent>();

        for (auto entity : view)
        {
            handles.push_back(new EntityHandle(entity, this));
        }
        return handles;
    }

    glm::mat4 Scene::GetWorldMatrix(entt::entity entity) const
    {
        const auto& tr = registry_.get<TransformComponent>(entity);
        glm::mat4 mat = tr.LocalMatrix();
        if (tr.parent != entt::null)
        {
            mat = GetWorldMatrix(tr.parent) * mat;
        }
        return mat;
    }
} // ProEngine
