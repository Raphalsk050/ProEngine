#pragma once
#include <string>
#include <entt.hpp>

#include "fwd.hpp"
#include "Core/Timestep.h"
#include "Core/Event/Event.h"

namespace ProEngine
{
    class EntityHandle;

    class Scene
    {
    public:
        Scene() = default;
        EntityHandle CreateEntity(const std::string& name = "Entity");
        void OnUpdate(Timestep ts);
        void LateUpdate(Timestep ts);
        void OnEvent(Event& e);
        std::vector<EntityHandle*> GetAllEntities();
        entt::registry& GetRegistry() { return registry_; }

        glm::mat4 GetWorldMatrix(entt::entity entity) const;

    private:
        friend class EntityHandle;
        entt::registry registry_;
    };
} // ProEngine
