#pragma once
#include "imnodes.h"
#include "Core/Editor/Frame/MaterialEditor/MaterialNodeTypes.h"

class NodeEditor;

namespace ProEngine
{
    struct UiNode;
    struct Node;

    class Multiply
    {
    public:
        static void CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos);
        static void RenderNode(const UiNode& node, Graph<Node>& graph);
        static void Evaluate(std::stack<float>& value_stack);
        static void DeleteNode(Graph<Node>& graph, std::__wrap_iter<UiNode*> ui_node_iter);
    };
}
