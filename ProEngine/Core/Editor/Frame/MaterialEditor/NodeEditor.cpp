#include "NodeEditor.h"
#include "Core/Input/Input.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/AddNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/MultiplyNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/TimeNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/CosNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/InputFloat.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/OutputNode.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/SineNode.h"
#include "spdlog/fmt/bundled/args.h"

namespace ProEngine
{
    NodeEditor::NodeEditor(): root_node_id_(0), minimap_location_(0), current_time_seconds_(0)
    {
    }

    NodeEditor::~NodeEditor() = default;

    void NodeEditor::OnAttach()
    {
        Layer::OnAttach();
        ImNodes::CreateContext();
        ImNodesIO& io = ImNodes::GetIO();
        io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
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
        if (!opened_) return;

        SetupWindow();
        if (ImGui::Begin("Node Editor"), &opened_)
        {
            RenderNodeEditor();

            ImGui::End();
        }

        const ImU32 color = root_node_id_ != -1 ? Evaluate(graph_, root_node_id_) : IM_COL32(255, 20, 147, 255);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
        ImGui::SetNextWindowSize(ImVec2(100, 100));
        if (ImGui::Begin("output color"))
        {
            ImGui::End();
        }
        ImGui::PopStyleColor();
    }

    void NodeEditor::OnEvent(Event& event)
    {
        Layer::OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(NodeEditor::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(NodeEditor::OnKeyReleased));
    }

    void NodeEditor::Open()
    {
        opened_ = true;
    }

    void NodeEditor::Close()
    {
        opened_ = false;
    }

    void NodeEditor::ToggleWindow()
    {
        opened_ = !opened_;
    }

    bool NodeEditor::OnKeyPressed(KeyPressedEvent& e)
    {
        bool pressed = (!e.IsRepeat());
        if (!key_states_.contains(e.GetKeyCode()))
        {
            key_states_.emplace(e.GetKeyCode(), false);
        }

        if (!key_states_[e.GetKeyCode()] && pressed)
            key_states_[e.GetKeyCode()] = true;

        return false;
    }

    bool NodeEditor::OnKeyReleased(KeyReleasedEvent& e)
    {
        if (!key_states_.contains(e.GetKeyCode()))
        {
            key_states_.emplace(e.GetKeyCode(), false);
        }

        if (key_states_[e.GetKeyCode()])
            key_states_[e.GetKeyCode()] = false;

        return false;
    }

    void NodeEditor::RenderNodeEditor()
    {
        ImNodes::BeginNodeEditor();
        const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
            ImNodes::IsEditorHovered() && key_states_[Key::A];

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
        {
            ImGui::OpenPopup("EditorOptions");
        }


        SetupPopup();

        ImGui::PopStyleVar();

        for (const UiNode& node : nodes_)
        {
            switch (node.type)
            {
            case UiNodeType::add:
                {
                    Add::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::multiply:
                {
                    Multiply::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::output:
                {
                    Output::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::input_float:
                {
                    InputFloat::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::sine:
                {
                    SineNode::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::cos:
                {
                    CosNode::RenderNode(node, graph_);
                }
                break;
            case UiNodeType::time:
                {
                    TimeNode::RenderNode(node, graph_);
                }
                break;
            }
        }

        for (const auto& edge : graph_.edges())
        {
            // If edge doesn't start at value, then it's an internal edge, i.e.
            // an edge which links a node's operation to its input. We don't
            // want to render node internals with visible links.
            if (graph_.node(edge.from).type != NodeType::value)
                continue;

            ImNodes::Link(edge.id, edge.from, edge.to);
        }

        ImNodes::MiniMap(0.2f, minimap_location_);


        ImNodes::EndNodeEditor();

        {
            int start_attr, end_attr;
            if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
            {
                const NodeType start_type = graph_.node(start_attr).type;
                const NodeType end_type = graph_.node(end_attr).type;

                const bool valid_link = start_type != end_type;
                if (valid_link)
                {
                    // Ensure the edge is always directed from the value to
                    // whatever produces the value
                    if (start_type != NodeType::value)
                    {
                        std::swap(start_attr, end_attr);
                    }
                    graph_.insert_edge(start_attr, end_attr);
                }

                PENGINE_CORE_INFO("{0}", valid_link ? "Link created" : "Link removed");
            }
        }

        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                graph_.erase_edge(link_id);
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedLinks();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedLinks(selected_links.data());
                for (const int edge_id : selected_links)
                {
                    graph_.erase_edge(edge_id);
                }
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedNodes();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (const int node_id : selected_nodes)
                {
                    graph_.erase_node(node_id);
                    auto iter = std::find_if(
                        nodes_.begin(), nodes_.end(), [node_id](const UiNode& node) -> bool
                        {
                            return node.id == node_id;
                        });
                    // Erase any additional internal nodes
                    switch (iter->type)
                    {
                    case UiNodeType::add:
                        graph_.erase_node(iter->ui.add.lhs);
                        graph_.erase_node(iter->ui.add.rhs);
                        break;
                    case UiNodeType::multiply:
                        graph_.erase_node(iter->ui.multiply.lhs);
                        graph_.erase_node(iter->ui.multiply.rhs);
                        break;
                    case UiNodeType::output:
                        graph_.erase_node(iter->ui.output.r);
                        graph_.erase_node(iter->ui.output.g);
                        graph_.erase_node(iter->ui.output.b);
                        root_node_id_ = -1;
                        break;
                    case UiNodeType::sine:
                        SineNode::DeleteNode(graph_, iter);
                        break;
                    default:
                        break;
                    }
                    nodes_.erase(iter);
                }
            }
        }
    }

    void NodeEditor::SetupWindow()
    {
        ImGui::SetNextWindowSizeConstraints(
            default_window_size_, // tamanho mínimo
            ImVec2(FLT_MAX, FLT_MAX) // tamanho máximo (pode usar FLT_MAX para “sem limite”)
        );
    }

    void NodeEditor::SetupPopup()
    {
        if (ImGui::BeginPopup("EditorOptions"))
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            if (ImGui::MenuItem("Add"))
            {
                Add::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("Sine"))
            {
                SineNode::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("Cos"))
            {
                CosNode::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("Multiply"))
            {
                Multiply::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("Time"))
            {
                TimeNode::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("InputFloat"))
            {
                InputFloat::CreateNode(graph_, nodes_, click_pos);
            }

            if (ImGui::MenuItem("Output"))
            {
                Output::CreateNode(graph_, nodes_, click_pos);
            }
            ImGui::EndPopup();
        }
    }
}
