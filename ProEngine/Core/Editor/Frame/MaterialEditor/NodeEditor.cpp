#include "NodeEditor.h"

#include "imnodes.h"

namespace ProEngine
{
    NodeEditor::NodeEditor()
    {
        debug_name_ = "Node Editor";
    }

    NodeEditor::~NodeEditor()
    {
    }

    void NodeEditor::OnAttach()
    {
        Layer::OnAttach();
        ImNodes::CreateContext();
        SetupDemoGraph();
    }

    void NodeEditor::OnDetach()
    {
        Layer::OnDetach();
    }

    void NodeEditor::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void NodeEditor::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        RenderNodeEditor();
    }

    void NodeEditor::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void NodeEditor::AddNode(const std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs)
    {
        ImVec2 mouse_pos = ImVec2(mouse_relative_position_);
        MaterialNode node;
        node.id = current_node_id_id_++;
        node.name = name;
        node.inputs = inputs;
        node.outputs = outputs;
        node.position = mouse_pos;
        AddNode(node);
    }

    void NodeEditor::AddNode(const MaterialNodeType& node_type)
    {
        auto mouse_pos = ImVec2(mouse_relative_position_);
        MaterialNode node;
        node.id = current_node_id_id_++;
        node.name = node_type.node_name;
        node.inputs = node_type.inputs;
        node.outputs = node_type.outputs;
        node.position = mouse_pos;
        AddNode(node);
    }

    void NodeEditor::AddNode(const MaterialNode& node)
    {
        graph_.nodes.push_back(node);
        ImNodes::SetNodeGridSpacePos(node.id, node.position);
    }

    void NodeEditor::RenderNodeEditor()
    {
        ImGui::Begin("Material Editor");

        ImNodes::BeginNodeEditor();
        window_position_ = ImGui::GetWindowPos();
        mouse_absolute_position_ = ImGui::GetMousePos();
        mouse_relative_position_ = ImVec2(mouse_absolute_position_.x - window_position_.x, mouse_absolute_position_.y - window_position_.y);

        // rendering a single node
        for (auto& node : graph_.nodes)
        {
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.name.c_str());
            ImNodes::EndNodeTitleBar();

            for (const auto& input : node.inputs)
            {
                ImNodes::BeginInputAttribute(std::hash<std::string>{}(input + std::to_string(node.id)));
                ImGui::Text("%s", input.c_str());
                ImNodes::EndInputAttribute();
            }

            for (const auto& output : node.outputs)
            {
                ImNodes::BeginOutputAttribute(std::hash<std::string>{}(output + std::to_string(node.id)));
                ImGui::Text("%s", output.c_str());
                ImNodes::EndOutputAttribute();
            }

            ImNodes::EndNode();
        }

        ImNodes::EndNodeEditor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            ImGui::ClosePopupsOverWindow(ImGui::GetCurrentWindow(), true);
            ImGui::OpenPopup("NodeEditorContextMenu");
        }

        if (ImGui::BeginPopup("NodeEditorContextMenu"))
        {
            if (ImGui::MenuItem("Add Texture2D Node"))
            {
                AddNode(Texture2DNode());
            }
            if (ImGui::MenuItem("Add Multiply Node"))
            {
                AddNode(MultiplyNode());
            }
            if (ImGui::MenuItem("Add LitMaster Node"))
            {
                AddNode(LitMasterNode());
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void NodeEditor::SetupDemoGraph()
    {
        AddNode(Texture2DNode());
        AddNode(MultiplyNode());
        AddNode(LitMasterNode());
    }

    void NodeEditor::Open()
    {
        opened_ = true;
    }

    void NodeEditor::Close()
    {
        opened_ = false;
    }

    void NodeEditor::ToggleWindow()
    {
        if (opened_)
        {
            Close();
        }
        else
        {
            Open();
        }
    }

    // Shader generation (simplified):
    std::string NodeEditor::GenerateShaderFromGraph(const MaterialGraph& graph)
    {
        // This function would resolve connections and topologically sort nodes,
        // then output GLSL code per node
        std::string shader = "#version 410 core\n";
        shader += "out vec4 FragColor;\nvoid main() {\n";
        shader += "    vec3 baseColor = vec3(1.0);\n"; // Placeholder
        shader += "    FragColor = vec4(baseColor, 1.0);\n";
        shader += "}";
        return shader;
    }
}
