#include "SineNode.h"
#include "Core/Editor/Frame/MaterialEditor/NodeEditor.h"

namespace ProEngine
{
    void SineNode::CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos)
    {
        const Node value(NodeType::value, 0.f);
        const Node op(NodeType::sine);

        UiNode ui_node;
        ui_node.type = UiNodeType::sine;
        ui_node.ui.sine.input = graph.insert_node(value);
        ui_node.id = graph.insert_node(op);

        graph.insert_edge(ui_node.id, ui_node.ui.sine.input);

        nodes.push_back(ui_node);
        ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
    }

    void SineNode::RenderNode(const UiNode& node, Graph<Node>& graph)
    {
        const float node_width = 100.0f;
        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("sine");
        ImNodes::EndNodeTitleBar();

        {
            ImNodes::BeginInputAttribute(node.ui.sine.input);
            const float label_width = ImGui::CalcTextSize("number").x;
            ImGui::TextUnformatted("number");
            if (graph.num_edges_from_node(node.ui.sine.input) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat(
                    "##hidelabel",
                    &graph.node(node.ui.sine.input).value,
                    0.01f,
                    0.f,
                    1.0f);
                ImGui::PopItemWidth();
            }
            ImNodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            ImNodes::BeginOutputAttribute(node.id);
            const float label_width = ImGui::CalcTextSize("output").x;
            ImGui::Indent(node_width - label_width);
            ImGui::TextUnformatted("output");
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }

    void SineNode::Evaluate(std::stack<float>& value_stack)
    {
        const float x = value_stack.top();
        value_stack.pop();
        const float res = std::abs(std::sin(x));
        value_stack.push(res);
    }

    void SineNode::DeleteNode(Graph<Node>& graph, __wrap_iter<UiNode*> ui_node_iter)
    {
        graph.erase_node(ui_node_iter->ui.sine.input);
    }
}
