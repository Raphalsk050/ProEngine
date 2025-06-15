#include "Core/Editor/Frame/Hierarchy/HierarchyInspector.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/EntityHandle.h"
#include "Core/Application/Application.h"
#include <imgui.h>

namespace ProEngine
{
    HierarchyInspector::HierarchyInspector()
    {
    }

    HierarchyInspector::~HierarchyInspector() = default;

    void HierarchyInspector::OnAttach()
    {
        Layer::OnAttach();
        registry_ = &Application::Get().GetActiveScene()->GetRegistry();
        active_scene_ = Application::Get().GetActiveScene();
    }

    void HierarchyInspector::OnDetach()
    {
        Layer::OnDetach();
    }

    void HierarchyInspector::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void HierarchyInspector::OnImGuiRender()
    {
        Layer::OnImGuiRender();

        if (!opened_) return;

        if (ImGui::Begin("Hierarchy", &opened_))
        {
            // Atualiza a referência da scene ativa (caso tenha mudado)
            if (active_scene_ != Application::Get().GetActiveScene())
            {
                active_scene_ = Application::Get().GetActiveScene();
                registry_ = &active_scene_->GetRegistry();
            }

            // Botão para criar nova entidade root
            if (ImGui::Button("Create Entity"))
            {
                active_scene_->CreateEntity("New Entity");
            }

            ImGui::Separator();

            // Pega todas as entidades
            auto entities = active_scene_->GetAllEntities();

            // Desenha apenas entidades root (sem parent)
            for (auto& entity : entities)
            {
                const auto& transform = entity->GetComponent<TransformComponent>();
                if (transform.parent == entt::null)
                {
                    DrawEntity(*entity);
                }
            }

            // Context menu no espaço vazio
            if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
            {
                if (ImGui::MenuItem("Create Empty"))
                {
                    active_scene_->CreateEntity("Empty Entity");
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void HierarchyInspector::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void HierarchyInspector::Open()
    {
        opened_ = true;
    }

    void HierarchyInspector::Close()
    {
        opened_ = false;
    }

    void HierarchyInspector::ToggleWindow()
    {
        opened_ = !opened_;
    }

    void HierarchyInspector::DrawEntity(EntityHandle& entity)
    {
        // Pega o nome da entidade
        std::string name = "Entity";
        const auto& tag_component = entity.GetComponent<TagComponent>();
        name = tag_component.tag;

        // Adiciona o ID da entidade para garantir uniqueness
        std::string label = name + "##" + std::to_string(entt::to_integral(entity.Raw()));

        const auto& transform = entity.GetComponent<TransformComponent>();
        bool has_children = transform.first_child != entt::null;

        ImGuiTreeNodeFlags node_flags = flags_;

        // Verifica se está selecionado
        bool is_selected = (selected_entity_ == entity.Raw());
        if (is_selected)
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        // Se não tem filhos, é uma folha
        if (!has_children)
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf;
        }

        bool node_open = ImGui::TreeNodeEx(label.c_str(), node_flags);

        // Handle de seleção ao clicar
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            selected_entity_ = entity.Raw();
            if (entity_handle_)
            {
                *entity_handle_ = entity;
            }
        }

        // Drag & Drop source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity, sizeof(EntityHandle));
            ImGui::Text("Move %s", name.c_str());
            ImGui::EndDragDropSource();
        }

        // Drag & Drop target
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
            {
                EntityHandle* dragged_entity = (EntityHandle*)payload->Data;

                // Não permite que uma entidade seja parent de si mesma
                if (dragged_entity->Raw() != entity.Raw())
                {
                    // Não permite que uma entidade seja parent de seus próprios filhos (evita ciclos)
                    if (!IsDescendantOf(*dragged_entity, entity))
                    {
                        dragged_entity->SetParent(entity);
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        // Context menu (botão direito)
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Create Child"))
            {
                EntityHandle child = active_scene_->CreateEntity("Child Entity");
                child.SetParent(entity);
            }

            if (ImGui::MenuItem("Duplicate"))
            {
                DuplicateEntity(entity);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Rename"))
            {
                // TODO: Implementar renomeação inline
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Delete"))
            {
                entities_to_delete_.push_back(entity);
            }

            ImGui::EndPopup();
        }

        // Se o nó está aberto, desenha os filhos
        if (node_open)
        {
            // Desenha todos os filhos
            if (has_children)
            {
                entt::entity current_child_entity = transform.first_child;

                while (current_child_entity != entt::null)
                {
                    EntityHandle current_child(current_child_entity, active_scene_);
                    DrawEntity(current_child);

                    const auto& child_transform = current_child.GetComponent<TransformComponent>();
                    current_child_entity = child_transform.next_sibling;
                }
            }

            ImGui::TreePop();
        }
    }

    void HierarchyInspector::DeleteEntity(EntityHandle& entity)
    {
        if (!entity.Valid()) return;

        // Primeiro, remove todos os filhos recursivamente
        auto children = entity.GetChildren();
        for (auto& child : children)
        {
            DeleteEntity(child);
        }

        // Remove da hierarquia (definindo parent como null)
        entity.SetParent(EntityHandle{});

        // Se era a entidade selecionada, limpa a seleção
        if (selected_entity_ == entity.Raw())
        {
            selected_entity_ = entt::null;
            if (entity_handle_)
            {
                *entity_handle_ = EntityHandle{};
            }
        }

        // Finalmente, destroi a entidade
        registry_->destroy(entity.Raw());
    }

    void HierarchyInspector::DuplicateEntity(EntityHandle& entity)
    {
        // Cria uma nova entidade com os mesmos componentes
        EntityHandle new_entity = active_scene_->CreateEntity(entity.GetComponent<TagComponent>().tag + " (Copy)");

        // Copia o TransformComponent
        const auto& transform = entity.GetComponent<TransformComponent>();
        auto& new_transform = new_entity.GetComponent<TransformComponent>();
        new_transform.position = transform.position;
        new_transform.rotation = transform.rotation;
        new_transform.scale = transform.scale;

        // Define o mesmo parent
        if (transform.parent != entt::null)
        {
            EntityHandle parent(transform.parent, active_scene_);
            new_entity.SetParent(parent);
        }

        // TODO: Copiar outros componentes conforme necessário

        // Duplica recursivamente todos os filhos
        auto children = entity.GetChildren();
        for (auto& child : children)
        {
            EntityHandle duplicated_child = DuplicateEntityRecursive(child);
            duplicated_child.SetParent(new_entity);
        }
    }

    EntityHandle HierarchyInspector::DuplicateEntityRecursive(EntityHandle& entity)
    {
        EntityHandle new_entity = active_scene_->CreateEntity(entity.GetComponent<TagComponent>().tag);

        // Copia o TransformComponent
        const auto& transform = entity.GetComponent<TransformComponent>();
        auto& new_transform = new_entity.GetComponent<TransformComponent>();
        new_transform.position = transform.position;
        new_transform.rotation = transform.rotation;
        new_transform.scale = transform.scale;

        // Duplica recursivamente todos os filhos
        auto children = entity.GetChildren();
        for (auto& child : children)
        {
            EntityHandle duplicated_child = DuplicateEntityRecursive(child);
            duplicated_child.SetParent(new_entity);
        }

        return new_entity;
    }

    bool HierarchyInspector::IsDescendantOf(EntityHandle& potential_descendant, EntityHandle& potential_ancestor)
    {
        EntityHandle current = potential_descendant;

        while (current.Valid())
        {
            const auto& transform = current.GetComponent<TransformComponent>();
            if (transform.parent == entt::null)
                break;

            if (transform.parent == potential_ancestor.Raw())
                return true;

            current = EntityHandle(transform.parent, active_scene_);
        }

        return false;
    }

    void HierarchyInspector::ProcessDeletions()
    {
        for (auto& entity : entities_to_delete_)
        {
            DeleteEntity(entity);
        }
        entities_to_delete_.clear();
    }

} // ProEngine