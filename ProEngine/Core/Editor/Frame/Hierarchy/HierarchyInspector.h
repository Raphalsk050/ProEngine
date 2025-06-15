#pragma once
#include "Core/Layer/Layer.h"
#include "Core/Scene/Scene.h"
#include <entt.hpp>
#include <vector>

namespace ProEngine
{
    class EntityHandle;

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

        void SetSelectedEntityHandle(EntityHandle* handle) { entity_handle_ = handle; }

    private:
        void DrawEntity(EntityHandle& entity);
        void DeleteEntity(EntityHandle& entity);
        void DuplicateEntity(EntityHandle& entity);
        EntityHandle DuplicateEntityRecursive(EntityHandle& entity);
        bool IsDescendantOf(EntityHandle& potential_descendant, EntityHandle& potential_ancestor);
        void ProcessDeletions();

    private:
        bool opened_ = false;
        entt::registry* registry_ = nullptr;
        Scene* active_scene_;

        entt::entity selected_entity_ = entt::null;
        EntityHandle* entity_handle_ = nullptr;

        std::vector<EntityHandle> entities_to_delete_;

        ImGuiTreeNodeFlags flags_ = ImGuiTreeNodeFlags_OpenOnArrow |
                                   ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                   ImGuiTreeNodeFlags_SpanAvailWidth;
    };
}