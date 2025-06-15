#include "EntityHandle.h"

namespace ProEngine
{
    EntityHandle::EntityHandle(entt::entity handle, Scene* scene) : handle_(handle), scene_(scene)
    {
    }

    void EntityHandle::SetParent(EntityHandle parent, bool keep_world_position)
    {
        auto& tr = GetComponent<TransformComponent>();
        entt::entity new_parent = parent.handle_;
        //if (tr.parent == new_parent) return;

        glm::vec3 world_pos{0.0f};
        if (keep_world_position)
        {
            glm::mat4 world = scene_->GetWorldMatrix(handle_);
            world_pos = glm::vec3(world[3]);
        }

        if (tr.parent != entt::null)
        {
            auto& parent_tr = scene_->registry_.get<TransformComponent>(tr.parent);
            if (parent_tr.first_child == handle_)
                parent_tr.first_child = tr.next_sibling;
            if (tr.prev_sibling != entt::null)
                scene_->registry_.get<TransformComponent>(tr.prev_sibling).next_sibling = tr.next_sibling;
            if (tr.next_sibling != entt::null)
                scene_->registry_.get<TransformComponent>(tr.next_sibling).prev_sibling = tr.prev_sibling;
        }

        tr.parent = new_parent;
        tr.next_sibling = tr.prev_sibling = entt::null;

        if (new_parent != entt::null)
        {
            auto& parent_tr = scene_->registry_.get<TransformComponent>(new_parent);
            if (parent_tr.first_child != entt::null)
                scene_->registry_.get<TransformComponent>(parent_tr.first_child).prev_sibling = handle_;
            tr.next_sibling = parent_tr.first_child;
            parent_tr.first_child = handle_;
        }

        if (keep_world_position)
        {
            glm::mat4 parent_world = new_parent != entt::null ? scene_->GetWorldMatrix(new_parent) : glm::mat4(1.0f);
            glm::vec4 local = glm::inverse(parent_world) * glm::vec4(world_pos, 1.0f);
            tr.position = glm::vec3(local);
        }
    }

    std::vector<EntityHandle> EntityHandle::GetChildren() const
    {
        std::vector<EntityHandle> children;
        const auto& tr = scene_->registry_.get<TransformComponent>(handle_);
        entt::entity child = tr.first_child;
        while (child != entt::null)
        {
            children.emplace_back(child, scene_);
            child = scene_->registry_.get<TransformComponent>(child).next_sibling;
        }
        return children;
    }

    void EntityHandle::SetPosition(const glm::vec3& position)
    {
        GetComponent<TransformComponent>().position = position;
    }

    glm::vec3 EntityHandle::GetPosition() const
    {
        return scene_->registry_.get<TransformComponent>(handle_).position;
    }

    void EntityHandle::SetRotation(const glm::vec3& rotation)
    {
        GetComponent<TransformComponent>().rotation = rotation;
    }

    glm::vec3 EntityHandle::GetRotation() const
    {
        return scene_->registry_.get<TransformComponent>(handle_).rotation;
    }

    void EntityHandle::SetScale(const glm::vec3& scale)
    {
        GetComponent<TransformComponent>().scale = scale;
    }

    glm::vec3 EntityHandle::GetScale() const
    {
        return scene_->registry_.get<TransformComponent>(handle_).scale;
    }

    bool EntityHandle::Valid() const
    {
    }
} // ProEngine
