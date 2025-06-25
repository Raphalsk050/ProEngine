#pragma once
#include <stdio.h>
#include <imnodes.h>
#include <iterator>

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

        static void DeleteNode(Graph<Node>& graph, __wrap_iter<UiNode*> ui_node_iter);
    };
}
