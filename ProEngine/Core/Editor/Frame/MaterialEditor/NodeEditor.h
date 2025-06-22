#pragma once
#include <vector>
#include "imgui.h"
#include "MaterialNodeTypes.h"
#include "Core/Layer/Layer.h"


namespace ProEngine
{
    struct Connection
    {
        int id;
        int start_attr;
        int end_attr;
    };

    struct MaterialNode
    {
        int id;
        std::string name;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
        ImVec2 position;
    };

    struct MaterialGraph
    {
        std::vector<MaterialNode> nodes;
        std::vector<Connection> connections;
    };

    using AttrMap = std::unordered_map<int, std::pair<MaterialNode*, int>>;

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
        void AddNode(const std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs);
        void AddNode(const MaterialNode& node);
        void AddNode(const MaterialNodeType& node_type);
        void RenderNodeEditor();
        void SetupDemoGraph();
        void RenderConnections();
        std::string GenerateShaderFromGraph(const MaterialGraph& graph);
        void Open();
        void Close();
        void ToggleWindow();

    private:
        MaterialGraph graph_;
        bool opened_ = true;
        int current_node_id_id_ = 1;
        bool is_popup_opened_ = false;
        bool request_context_menu_ = false;
        int next_link_id_ = 0;
        std::unordered_map<int, std::pair<MaterialNode*, int>> attrToNodeAttr;
        static void PrintAllConnectionValues( const AttrMap& attrToNodeAttr, const std::vector<Connection>& connections );

        ImVec2 mouse_relative_position_;
        ImVec2 mouse_absolute_position_;
        ImVec2 window_position_;
    };
}
