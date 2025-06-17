#pragma once
#include "Core/Layer/Layer.h"
#include "Core/Scene/Scene.h"
#include <vector>

#include "Core/Editor/SimpleAnimatedPopup.h"
#include "Core/Scene/EntityHandle.h"

namespace ProEngine
{
    class EntityHandle;

    struct SelectedEntityValues
    {
        glm::vec3 selected_entity_position = glm::vec3(0.0f);
        glm::vec3 selected_entity_rotation = glm::vec3(0.0f);
        glm::vec3 selected_entity_scale = glm::vec3(1.0f);
    };

    class HierarchyInspector : public Layer
    {
    public:
        HierarchyInspector();
        ~HierarchyInspector();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

        void Open();
        void Close();
        void ToggleWindow();

        void SetSelectedEntityHandle(EntityHandle* handle) { entity_handle_ = *handle; }

    private:
        void DrawEntity(EntityHandle& entity);
        void DrawEntityPropertiesWindow();
        void DeleteEntity(EntityHandle& entity);
        void DuplicateEntity(EntityHandle& entity);
        EntityHandle DuplicateEntityRecursive(EntityHandle& entity);
        bool IsDescendantOf(EntityHandle& potential_descendant, EntityHandle& potential_ancestor);
        void ProcessDeletions();

    private:
        bool opened_ = false;
        entt::registry* registry_;
        Scene* active_scene_;
        entt::entity selected_entity_ = entt::null;
        EntityHandle entity_handle_;
        EntityHandle* selected_entity_handle_ = nullptr;
        SelectedEntityValues selected_entity_transform_;

        std::vector<EntityHandle> entities_to_delete_;

        ImGuiTreeNodeFlags flags_ = ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth;

    private:
        void CopyAllComponents(entt::registry& registry, entt::entity src, entt::entity dst);
    };
}
