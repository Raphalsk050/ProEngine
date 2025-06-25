#pragma once
#include <cstdio>

#include "imnodes.h"
#include "Core/Editor/Frame/MaterialEditor/MaterialNodeTypes.h"
#include "Core/Editor/Frame/MaterialEditor/NodeEditor.h"

class NodeEditor;

namespace ProEngine
{
    class InputFloat
    {
    public:
        static void inline CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, ImVec2 click_pos)
        {
            const Node value(NodeType::value, 0.f);
            const Node op(NodeType::input_float);

            UiNode ui_node{};
            ui_node.type = UiNodeType::input_float;
            ui_node.ui.input_float.a = graph.insert_node(value);
            ui_node.id = graph.insert_node(op);

            graph.insert_edge(ui_node.id, ui_node.ui.input_float.a);

            nodes.push_back(ui_node);
            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
        }

        static void inline RenderNode(const UiNode& node, Graph<Node>& graph)
        {
            const float node_width = 100.f;
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("float");
            ImNodes::EndNodeTitleBar();
            {
                ImNodes::BeginStaticAttribute(node.ui.input_float.a);
                const float label_width = ImGui::CalcTextSize("float").x;
                ImGui::TextUnformatted("float");
                if (graph.num_edges_from_node(node.ui.input_float.a) == 0ull)
                {
                    ImGui::SameLine();
                    ImGui::PushItemWidth(node_width - label_width);
                    ImGui::DragFloat("##hidelabel", &graph.node(node.ui.input_float.a).value, 0.01f);
                    ImGui::PopItemWidth();
                }
                ImNodes::EndStaticAttribute();

                ImGui::Spacing();

                {
                    ImNodes::BeginOutputAttribute(node.id);
                    const float label_width = ImGui::CalcTextSize("value").x;
                    ImGui::Indent(node_width - label_width);
                    ImGui::TextUnformatted("value");
                    ImNodes::EndOutputAttribute();
                }
            }

            ImNodes::EndNode();
        }

        static void inline DeleteNode(const Graph<Node>& graph, std::vector<UiNode>::iterator iter)
        {

        }
    };
}
