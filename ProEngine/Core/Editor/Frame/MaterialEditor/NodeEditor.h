#pragma once
#include <vector>
#include "imgui.h"
#include "Core/Layer/Layer.h"


namespace ProEngine
{
    struct Connection {
        int inputNodeId;
        int outputNodeId;
        std::string inputSlot;
        std::string outputSlot;
    };

    struct MaterialNode {
        int id;
        std::string name;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
        ImVec2 position;
    };

    struct MaterialGraph {
        std::vector<MaterialNode> nodes;
        std::vector<Connection> connections;
    };
    class NodeEditor : public Layer{
    public:
        NodeEditor();
        ~NodeEditor() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
        void AddNode(const std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs);
        void RenderNodeEditor();
        void SetupDemoGraph();
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
    };
}
