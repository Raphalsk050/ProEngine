#pragma once
#include <stdio.h>
#include <iterator>
#include "imnodes.h"

#include "Core/Editor/Frame/MaterialEditor/NodeEditor.h"

class NodeEditor;

namespace ProEngine
{
    class Add
    {
    public:
        static void CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, ImVec2 click_pos);

        static void RenderNode(const UiNode& node, Graph<Node>& graph);

        static void Evaluate(std::stack<float>& value_stack);

        static void DeleteNode(Graph<Node>& graph, __wrap_iter<UiNode*> ui_node_iter);
    };
}
