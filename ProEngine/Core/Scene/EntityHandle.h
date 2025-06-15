#pragma once
#include "Core/Scene/Components.h"
#include "vec3.hpp"
#include "Core/Scene/Scene.h"

namespace ProEngine
{
    class Scene;

    class EntityHandle
    {
    public:
        EntityHandle() = default;
        EntityHandle(entt::entity handle, Scene* scene);
        EntityHandle(const EntityHandle&) = default;
        EntityHandle& operator=(const EntityHandle&) = default;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args);


        template <typename T>
        void RemoveComponent();

        template <typename T>
        T& GetComponent();

        void SetParent(EntityHandle parent, bool keep_world_position = false);
        std::vector<EntityHandle> GetChildren() const;

        void SetPosition(const glm::vec3& position);
        glm::vec3 GetPosition() const;
        void SetRotation(const glm::vec3& rotation);
        glm::vec3 GetRotation() const;
        void SetScale(const glm::vec3& scale);
        glm::vec3 GetScale() const;

        bool Valid() const;
        operator bool() const { return Valid(); }
        entt::entity Raw() const { return handle_; }

    private:
        entt::entity handle_{entt::null};
        Scene* scene_ = nullptr;

        friend class Scene;
        friend class HierarchyInspector;
    };
} // ProEngine

namespace ProEngine
{
    template <typename T, typename... Args>
    T& EntityHandle::AddComponent(Args&&... args)
    {
        return scene_->registry_.emplace<T>(handle_, std::forward<Args>(args)...);
    }

    template <typename T>
    void EntityHandle::RemoveComponent()
    {
        static_assert(!std::is_same_v<T, TransformComponent>, "Transform component cannot be removed");
        scene_->registry_.remove<T>(handle_);
    }

    template <typename T>
    T& EntityHandle::GetComponent()
    {
        return scene_->registry_.get<T>(handle_);
    }
}
