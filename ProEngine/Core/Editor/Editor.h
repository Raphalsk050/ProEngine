#pragma once
#include <functional>
#include <entt.hpp>

#include "Core/Scene/Components.h"
#include "Core/Editor/Frame/Hierarchy/EntityProperties/EditorPropertiesFactory.h"
#include "Frame/Hierarchy/EntityProperties/EditorVectorProperties.h"

namespace ProEngine
{
    class Editor
    {
    public:
        static EditorPropertiesFactory properties_factory;

        // singletone to prevent other editor instances
        static Editor* Get()
        {
            if (editor_ == nullptr)
                editor_ = new Editor();
            return editor_;
        }

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

            properties_factory.registerDrawer<TransformComponent>("Transform", [](void* ptr){
                auto& tc = *static_cast<TransformComponent*>(ptr);
                Vector3Field::RenderField(&tc.position, tc.id);
                Vector3Field::RenderField(&tc.rotation, tc.id + 3);
                Vector3Field::RenderField(&tc.scale, tc.id + 6);
                ImGui::Separator();
            });

            properties_factory.registerDrawer<TagComponent>("Tag", [](void* ptr){
                auto& tc = *static_cast<TagComponent*>(ptr);
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Tag: ");
                ImGui::SameLine();
                ImGui::Text(tc.tag.c_str());
            });

            properties_factory.registerDrawer<RendererComponent>("Renderer Component", [](void* ptr){
                auto& rc = *static_cast<RendererComponent*>(ptr);


                ImGui::Checkbox("##depth_test",&rc.depth_test);
                ImGui::AlignTextToFramePadding();
                ImGui::SameLine();
                ImGui::Text("Depth Test");

                ImGui::Checkbox("##culling",&rc.culling);
                ImGui::AlignTextToFramePadding();
                ImGui::SameLine();
                ImGui::Text("Culling");

                ImGui::Checkbox("##double_sided",&rc.double_sided);
                ImGui::AlignTextToFramePadding();
                ImGui::SameLine();
                ImGui::Text("Double Sided");
            });

            // this serves as the duplicate hierarchy function
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
