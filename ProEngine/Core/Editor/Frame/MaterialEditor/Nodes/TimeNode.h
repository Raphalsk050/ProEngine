#pragma once
#include <_stdio.h>
#include "imnodes.h"


namespace ProEngine
{
    class TimeNode
    {
    public:
        static void inline CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos)
        {
            UiNode ui_node{};
            ui_node.type = UiNodeType::time;
            ui_node.id = graph.insert_node(Node(NodeType::time));

            nodes.push_back(ui_node);
            ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
        }

        static void inline RenderNode(const UiNode& node, Graph<Node>& graph)
        {
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("time");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginOutputAttribute(node.id);
            ImGui::Text("Delta time");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();
        }
    };
}
