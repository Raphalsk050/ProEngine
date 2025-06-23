#pragma once
#include <vector>
#include "imgui.h"
#include "imgui_internal.h"
#include "imnodes.h"
#include "MaterialNodeTypes.h"
#include "Core/Layer/Layer.h"


namespace ProEngine
{
    enum class NodeType
    {
        add,
        multiply,
        output,
        sine,
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
        output,
        sine,
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
                int input;
            } sine;
        } ui;
    };


    class NodeEditor : public Layer
    {
    public:
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
        Graph<Node>            graph_;
        std::vector<UiNode>    nodes_;
        int                    root_node_id_;
        ImNodesMiniMapLocation minimap_location_;

    private:
        void RenderNodeEditor();
        void SetupWindow();
        void SetupPopup();
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
    };
}
