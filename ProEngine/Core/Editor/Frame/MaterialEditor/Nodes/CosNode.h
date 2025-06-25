#pragma once
#include <cstdio>
#include "imnodes.h"
#include "Core/Editor/Frame/MaterialEditor/MaterialNodeTypes.h"
#include "Core/Editor/Frame/MaterialEditor/NodeEditor.h"

namespace ProEngine
{
    class CosNode
    {
    public:
        static void CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos);

        static void RenderNode(const UiNode& node, Graph<Node>& graph);

        static void Evaluate(std::stack<float>& value_stack);

        static void DeleteNode(Graph<Node>& graph, std::vector<UiNode>::iterator UiNodePtr);
    };
}
