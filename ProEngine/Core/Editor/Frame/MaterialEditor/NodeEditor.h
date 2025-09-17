#pragma once
#include <vector>
#include "imgui.h"
#include "imnodes.h"
#include "MaterialNodeTypes.h"
#include "Core/Layer/Layer.h"
#include "Core/Editor/Frame/MaterialEditor/Nodes/SineNode.h"
#include "Nodes/AddNode.h"
#include "Nodes/CosNode.h"
#include "Nodes/MultiplyNode.h"
#include "Preview/MaterialPreview.h"


namespace ProEngine
{
    enum class NodeType
    {
        add,
        multiply,
        output,
        input_float,
        sine,
        cos,
        time,
        value
    };

    struct Node
    {
        NodeType type;
        float value;

        explicit Node(const NodeType t) : type(t), value(0.f)
        {
        }

        Node(const NodeType t, const float v) : type(t), value(v)
        {
        }
    };

    enum class UiNodeType
    {
        add,
        multiply,
        input_float,
        output,
        sine,
        cos,
        time,
    };

    struct UiNode
    {
        UiNodeType type;
        // The identifying id of the ui node. For add, multiply, sine, and time
        // this is the "operation" node id. The additional input nodes are
        // stored in the structs.
        int id;

        union
        {
            struct
            {
                int lhs, rhs;
            } add;

            struct
            {
                int lhs, rhs;
            } multiply;

            struct
            {
                int r, g, b;
            } output;

            struct
            {
                int a;
            } input_float;

            struct
            {
                int input;
            } sine;

            struct
            {
                int input;
            } cos;

            struct
            {
                int lhs, rhs;
            } time;
        } ui;
    };


    class NodeEditor : public Layer
    {
    public:
        inline bool HasEdge(int id) const
        {
            for (auto& e : graph_.edges())
                if (e.id == id) return true;
            return false;
        }

        inline ImU32 Evaluate(const Graph<Node>& graph, const int root_node) const
        {
            std::stack<int> postorder;
            dfs_traverse(
                graph, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

            std::stack<float> value_stack;
            while (!postorder.empty())
            {
                const int id = postorder.top();
                postorder.pop();
                const Node node = graph.node(id);

                switch (node.type)
                {
                case NodeType::add:
                    {
                        Add::Evaluate(value_stack);
                    }
                    break;
                case NodeType::multiply:
                    {
                        Multiply::Evaluate(value_stack);
                    }
                    break;
                case NodeType::sine:
                    {
                        SineNode::Evaluate(value_stack);
                    }
                    break;
                case NodeType::cos:
                    {
                        CosNode::Evaluate(value_stack);
                    }
                    break;
                case NodeType::time:
                    {
                        value_stack.push(current_time_seconds_);
                    }
                    break;
                case NodeType::value:
                    {
                        // If the edge does not have an edge connecting to another node, then just use the value
                        // at this node. It means the node's input pin has not been connected to anything and
                        // the value comes from the node's UI.
                        if (graph.num_edges_from_node(id) == 0ull)
                        {
                            value_stack.push(node.value);
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            // The final output node isn't evaluated in the loop -- instead we just pop
            // the three values which should be in the stack.
            assert(value_stack.size() == 3ull);
            const int b = static_cast<int>(255.f * std::clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
            value_stack.pop();
            const int g = static_cast<int>(255.f * std::clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
            value_stack.pop();
            const int r = static_cast<int>(255.f * std::clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
            value_stack.pop();

            return IM_COL32(r, g, b, 255);
        }

        NodeEditor();
        ~NodeEditor() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
        void Open();
        void Close();
        void ToggleWindow();

    private:
        bool opened_ = false;
        std::unordered_map<KeyCode, bool> key_states_;
        ImVec2 default_window_size_ = {400.0, 400.0};
        ImVec2 preview_size_ = {256.0f, 256.0f};
        Graph<Node> graph_;
        std::vector<UiNode> nodes_;
        int root_node_id_;
        ImNodesMiniMapLocation minimap_location_;
        float current_time_seconds_;

        Ref<class MaterialPreview> preview_;

    private:
        void RenderNodeEditor();
        void SetupWindow();
        void SetupPopup();
        void CreateNodeLinks();
        void DeleteNodes();
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
    };
}
