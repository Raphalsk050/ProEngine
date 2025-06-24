#pragma once
#include <_stdio.h>

#include "imnodes.h"
#include "Core/Editor/Frame/MaterialEditor/MaterialNodeTypes.h"
#include "Core/Editor/Frame/MaterialEditor/NodeEditor.h"

namespace ProEngine
{
    class Output
    {
    public:
        static int inline CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, ImVec2 click_pos)
        {
            const Node value(NodeType::value, 0.f);
            const Node op(NodeType::output);

            UiNode ui_node{};
            ui_node.type = UiNodeType::output;
            ui_node.ui.output.r = graph.insert_node(value);
            ui_node.ui.output.g = graph.insert_node(value);
            ui_node.ui.output.b = graph.insert_node(value);
            ui_node.id = graph.insert_node(op);

            graph.insert_edge(ui_node.id, ui_node.ui.output.r);
            graph.insert_edge(ui_node.id, ui_node.ui.output.g);
            graph.insert_edge(ui_node.id, ui_node.ui.output.b);

            nodes.push_back(ui_node);
            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
            return ui_node.id;
        }

        static void inline RenderNode(const UiNode& node, Graph<Node>& graph)
        {
            const float node_width = 100.0f;
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(45, 126, 194, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("output");
            ImNodes::EndNodeTitleBar();

            ImGui::Dummy(ImVec2(node_width, 0.f));
            {
                ImNodes::BeginInputAttribute(node.ui.output.r);
                const float label_width = ImGui::CalcTextSize("r").x;
                ImGui::TextUnformatted("r");
                if (graph.num_edges_from_node(node.ui.output.r) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat(
                        "##hidelabel", &graph.node(node.ui.output.r).value, 0.01f, 0.f, 1.0f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndInputAttribute();
            }

            ImGui::Spacing();

            {
                ImNodes::BeginInputAttribute(node.ui.output.g);
                const float label_width = ImGui::CalcTextSize("g").x;
                ImGui::TextUnformatted("g");
                if (graph.num_edges_from_node(node.ui.output.g) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat(
                        "##hidelabel", &graph.node(node.ui.output.g).value, 0.01f, 0.f, 1.f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndInputAttribute();
            }

            ImGui::Spacing();

            {
                ImNodes::BeginInputAttribute(node.ui.output.b);
                const float label_width = ImGui::CalcTextSize("b").x;
                ImGui::TextUnformatted("b");
                if (graph.num_edges_from_node(node.ui.output.b) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat(
                        "##hidelabel", &graph.node(node.ui.output.b).value, 0.01f, 0.f, 1.0f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndInputAttribute();
            }
            ImNodes::EndNode();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }

        static void inline DeleteNode(const Graph<Node>& graph, __wrap_iter<UiNode*> iter)
        {

        }
    };
}
