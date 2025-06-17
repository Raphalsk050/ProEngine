#pragma once
#include <functional>
#include "Core/Scene/Components.h"
#include "entt.hpp"

namespace ProEngine
{
    class Editor
    {
    public:
        using CloneFn = std::function<void(entt::registry&, entt::entity, entt::entity)>;
        inline static std::unordered_map<entt::id_type, CloneFn> clone_functions;

        template <typename T>
        inline static void register_cloner()
        {
            clone_functions[entt::type_hash<T>::value()] = [](entt::registry& registry, entt::entity src, entt::entity dst)
            {
                if (registry.all_of<T>(src))
                {
                    const auto& component = registry.get<T>(src);
                    if (!registry.all_of<T>(dst))
                    {
                        registry.emplace<T>(dst, component);
                    }
                    else
                    {
                        registry.replace<T>(dst, component);
                    }
                }
            };
        }
        inline static void Initialize()
        {
            if (editor_ == nullptr)
            {
                editor_ = new Editor();
            }

            register_cloner<TransformComponent>();
            register_cloner<CameraComponent>();
            register_cloner<LightComponent>();
            register_cloner<TagComponent>();
            register_cloner<CharacterControllerComponent>();
            register_cloner<ModelRendererComponent>();
            register_cloner<RendererComponent>();
        }

    private:
        Editor() = default;
        ~Editor() = default;
        inline static Editor* editor_;
    };
}
