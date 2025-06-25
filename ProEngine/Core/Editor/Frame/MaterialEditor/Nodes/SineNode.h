#pragma once
#include <cstdio>
#include <imnodes.h>

namespace ProEngine
{
    struct UiNode;
    struct Node;

    class SineNode
    {
    public:
        static void CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos);

        static void RenderNode(const UiNode& node, Graph<Node>& graph);

        static void Evaluate(std::stack<float>& value_stack);

        static void DeleteNode(Graph<Node>& graph, std::vector<UiNode>::iterator ui_node_iter);
    };
}
