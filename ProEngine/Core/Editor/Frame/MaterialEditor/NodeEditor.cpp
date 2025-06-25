#include "NodeEditor.h"
#include "Core/Input/Input.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/AddNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/MultiplyNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/TimeNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/CosNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/InputFloat.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/OutputNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/SineNode.h"
#include <spdlog/fmt/bundled/args.h>

namespace ProEngine
{
    NodeEditor::NodeEditor()
        : root_node_id_(0), minimap_location_(0), current_time_seconds_(0), opened_(false)
    {
    }

    NodeEditor::~NodeEditor() = default;

    void NodeEditor::OnAttach()
    {
        Layer::OnAttach();
        ImNodes::CreateContext();
        ImNodesIO& imnodes_io = ImNodes::GetIO();
        ImGuiIO& imgui_io = ImGui::GetIO();
        imnodes_io.LinkDetachWithModifierClick.Modifier = &imgui_io.KeyCtrl;
        imnodes_io.MultipleSelectModifier.Modifier = &imgui_io.KeyShift;
        imnodes_io.EmulateThreeButtonMouse.Modifier = &imgui_io.KeyAlt;
        // Cria nó de saída raiz
        root_node_id_ = Output::CreateNode(graph_, nodes_, ImVec2(0, 0));
    }

    void NodeEditor::OnDetach()
    {
        Layer::OnDetach();
    }

    void NodeEditor::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        current_time_seconds_ = Time::GetTime();
    }

    void NodeEditor::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        if (!opened_)
            return;

        SetupWindow();
        if (ImGui::Begin("Node Editor", &opened_))
        {
            RenderNodeEditor();
            ImGui::End();
        }
    }

    void NodeEditor::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(NodeEditor::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(NodeEditor::OnKeyReleased));
    }

    void NodeEditor::Open() { opened_ = true; }
    void NodeEditor::Close() { opened_ = false; }
    void NodeEditor::ToggleWindow() { opened_ = !opened_; }

    bool NodeEditor::OnKeyPressed(KeyPressedEvent& e)
    {
        if (!e.IsRepeat())
            key_states_[e.GetKeyCode()] = true;
        return false;
    }

    bool NodeEditor::OnKeyReleased(KeyReleasedEvent& e)
    {
        key_states_[e.GetKeyCode()] = false;
        return false;
    }

    void NodeEditor::RenderNodeEditor()
    {
        ImNodes::BeginNodeEditor();
        int margin = 100;
        const ImU32 color = (root_node_id_ != -1)
                                ? Evaluate(graph_, root_node_id_)
                                : IM_COL32(255, 20, 147, 255);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x - margin * 0.85, ImGui::GetWindowSize().y - margin));
        ImGui::BeginChild("Preview", ImVec2(300, 300), true,
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
                          | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMouseInputs);

        ImGui::SameLine(20);
        ImGui::Text("Preview");

        ImGui::EndChild();
        ImGui::PopStyleColor();

        // Pop-up de contexto
        const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
            && ImNodes::IsEditorHovered()
            && key_states_[Key::A];
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
            ImGui::OpenPopup("EditorOptions");
        SetupPopup();
        ImGui::PopStyleVar();

        // Renderiza cada nó
        for (const UiNode& node : nodes_)
        {
            switch (node.type)
            {
            case UiNodeType::add: Add::RenderNode(node, graph_);
                break;
            case UiNodeType::multiply: Multiply::RenderNode(node, graph_);
                break;
            case UiNodeType::output: Output::RenderNode(node, graph_);
                break;
            case UiNodeType::input_float: InputFloat::RenderNode(node, graph_);
                break;
            case UiNodeType::sine: SineNode::RenderNode(node, graph_);
                break;
            case UiNodeType::cos: CosNode::RenderNode(node, graph_);
                break;
            case UiNodeType::time: TimeNode::RenderNode(node, graph_);
                break;
            }
        }

        // Renderiza links visíveis
        for (const auto& edge : graph_.edges())
        {
            if (graph_.node(edge.from).type != NodeType::value)
                continue;
            ImNodes::Link(edge.id, edge.from, edge.to);
        }

        ImNodes::MiniMap(0.2f, minimap_location_);
        ImNodes::EndNodeEditor();

        CreateNodeLinks();
        DeleteNodes();
    }

    void NodeEditor::SetupWindow()
    {
        ImGui::SetNextWindowSizeConstraints(
            default_window_size_,
            ImVec2(FLT_MAX, FLT_MAX)
        );
    }

    void NodeEditor::SetupPopup()
    {
        if (!ImGui::BeginPopup("EditorOptions"))
            return;
        const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

        if (ImGui::MenuItem("Add")) Add::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("Sine")) SineNode::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("Cos")) CosNode::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("Multiply")) Multiply::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("Time")) TimeNode::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("InputFloat"))InputFloat::CreateNode(graph_, nodes_, click_pos);
        if (ImGui::MenuItem("Output")) Output::CreateNode(graph_, nodes_, click_pos);
        ImGui::EndPopup();
    }

    void NodeEditor::CreateNodeLinks()
    {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            auto start_type = graph_.node(start_attr).type;
            auto end_type = graph_.node(end_attr).type;
            bool valid_link = (start_type != end_type);
            if (valid_link)
            {
                if (start_type != NodeType::value)
                    std::swap(start_attr, end_attr);
                graph_.insert_edge(start_attr, end_attr);
            }
            PENGINE_CORE_INFO("{0}", valid_link ? "Link created" : "Invalid link");
        }
    }

    void NodeEditor::DeleteNodes()
    {
        // 1) Remoção por seleção de links
        int num_selected_links = ImNodes::NumSelectedLinks();
        if (num_selected_links > 0 && key_states_[Key::X])
        {
            std::vector<int> selected_links(num_selected_links);
            ImNodes::GetSelectedLinks(selected_links.data());
            for (int edge_id : selected_links)
            {
                if (HasEdge(edge_id))
                    graph_.erase_edge(edge_id);
            }
        }
        // 2) Remoção por desconexão padrão
        int destroyed_link_id;
        if (ImNodes::IsLinkDestroyed(&destroyed_link_id))
        {
            if (HasEdge(destroyed_link_id))
                graph_.erase_edge(destroyed_link_id);
        }

        // 3) Remoção de nós selecionados
        int num_selected_nodes = ImNodes::NumSelectedNodes();
        if (num_selected_nodes > 0 && key_states_[Key::X])
        {
            std::vector<int> selected_nodes(num_selected_nodes);
            ImNodes::GetSelectedNodes(selected_nodes.data());
            for (int node_id : selected_nodes)
            {
                auto iter = std::find_if(
                    nodes_.begin(), nodes_.end(),
                    [node_id](const UiNode& n) { return n.id == node_id; }
                );
                if (iter == nodes_.end())
                {
                    PENGINE_CORE_WARN("Trying to delete a node that doesn't exist: {}", node_id);
                    continue;
                }
                // Deleção específica por tipo
                switch (iter->type)
                {
                case UiNodeType::add: Add::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::multiply: Multiply::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::sine: SineNode::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::cos: CosNode::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::time: TimeNode::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::input_float: InputFloat::DeleteNode(graph_, iter);
                    break;
                case UiNodeType::output: Output::DeleteNode(graph_, iter);
                    root_node_id_ = -1;
                    break;
                default: break;
                }
                graph_.erase_node(node_id);
                nodes_.erase(iter);
            }
        }
    }
}
