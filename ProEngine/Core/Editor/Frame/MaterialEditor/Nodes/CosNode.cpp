#include "CosNode.h"

namespace ProEngine
{
    void CosNode::CreateNode(Graph<Node>& graph, std::vector<UiNode>& nodes, const ImVec2 click_pos)
    {
        const Node value(NodeType::value, 0.f);
        const Node op(NodeType::cos);

        UiNode ui_node;
        ui_node.type = UiNodeType::cos;
        ui_node.ui.sine.input = graph.insert_node(value);
        ui_node.id = graph.insert_node(op);

        graph.insert_edge(ui_node.id, ui_node.ui.sine.input);

        nodes.push_back(ui_node);
        ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
    }

    void CosNode::RenderNode(const UiNode& node, Graph<Node>& graph)
    {
        const float node_width = 100.0f;
        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("cos");
        ImNodes::EndNodeTitleBar();

        {
            ImNodes::BeginInputAttribute(node.ui.cos.input);
            const float label_width = ImGui::CalcTextSize("number").x;
            ImGui::TextUnformatted("number");
            if (graph.num_edges_from_node(node.ui.cos.input) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat(
                    "##hidelabel",
                    &graph.node(node.ui.cos.input).value,
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

    void CosNode::Evaluate(std::stack<float>& value_stack)
    {
        const float x = value_stack.top();
        value_stack.pop();
        const float res = std::abs(std::cos(x));
        value_stack.push(res);
    }

    void CosNode::DeleteNode(Graph<Node>& graph, std::vector<UiNode>::iterator ui_node_iter)
    {
        graph.erase_node(ui_node_iter->ui.cos.input);
    }
}
