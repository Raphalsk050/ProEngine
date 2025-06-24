#pragma once
#include <_stdio.h>

#include "imnodes.h"

class NodeEditor;

namespace ProEngine
{
    class Add
    {
    public:
        static void inline CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, ImVec2 click_pos)
        {
            const Node value(NodeType::value, 0.f);
            const Node op(NodeType::add);

            UiNode ui_node{};
            ui_node.type = UiNodeType::add;
            ui_node.ui.add.lhs = graph.insert_node(value);
            ui_node.ui.add.rhs = graph.insert_node(value);
            ui_node.id = graph.insert_node(op);

            graph.insert_edge(ui_node.id, ui_node.ui.add.lhs);
            graph.insert_edge(ui_node.id, ui_node.ui.add.rhs);

            nodes.push_back(ui_node);
            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
        }

        static void inline RenderNode(const UiNode& node, Graph<Node>& graph)
        {
            const float node_width = 100.f;
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("add");
            ImNodes::EndNodeTitleBar();
            {
                ImNodes::BeginInputAttribute(node.ui.add.lhs);
                const float label_width = ImGui::CalcTextSize("left").x;
                ImGui::TextUnformatted("left");
                if (graph.num_edges_from_node(node.ui.add.lhs) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat("##hidelabel", &graph.node(node.ui.add.lhs).value, 0.01f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndInputAttribute();
            }

            {
                ImNodes::BeginInputAttribute(node.ui.add.rhs);
                const float label_width = ImGui::CalcTextSize("right").x;
                ImGui::TextUnformatted("right");
                if (graph.num_edges_from_node(node.ui.add.rhs) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat("##hidelabel", &graph.node(node.ui.add.rhs).value, 0.01f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndInputAttribute();
            }

            ImGui::Spacing();

            {
                ImNodes::BeginOutputAttribute(node.id);
                const float label_width = ImGui::CalcTextSize("result").x;
                ImGui::Indent(node_width - label_width);
                ImGui::TextUnformatted("result");
                ImNodes::EndOutputAttribute();
            }

            ImNodes::EndNode();
        }
    };
}
