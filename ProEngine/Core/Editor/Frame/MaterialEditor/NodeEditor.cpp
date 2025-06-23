#include "NodeEditor.h"

#include "imgui_internal.h"
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
        graph_.connections.reserve(100);
        graph_.nodes.reserve(100);
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
        graph_.nodes.emplace_back(node);
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

            for (size_t i = 0; i < node.inputs.size(); ++i)
            {
                int attrId = std::hash<std::string>{}(node.inputs[i] + std::to_string(node.id));
                ImNodes::BeginInputAttribute(attrId);
                attrToNodeAttr[attrId] = {&node, static_cast<int>(i)};
                ImGui::Text("%s", node.inputs[i].c_str());
                ImNodes::EndInputAttribute();
            }

            for (size_t i = 0; i < node.outputs.size(); ++i)
            {
                int attrId = std::hash<std::string>{}(node.outputs[i] + std::to_string(node.id));
                ImNodes::BeginOutputAttribute(attrId);
                attrToNodeAttr[attrId] = {&node, static_cast<int>(i)};
                ImGui::Text("%s", node.outputs[i].c_str());
                ImNodes::EndOutputAttribute();
            }

            ImNodes::EndNode();
        }

        RenderConnections();

        ImNodes::EndNodeEditor();

        // logo após o EndNodeEditor, cheque:
        int start_attr = 0, end_attr = 0;
        bool from_snap = false;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr, &from_snap))
        {
            // start_attr → end_attr was connected right now
            int link_id = next_link_id_;
            auto connection = Connection({link_id, start_attr, end_attr});
            graph_.connections.emplace_back(connection);

            next_link_id_++;

            PENGINE_CORE_INFO("######## Connection made ########");
            PENGINE_CORE_INFO("|        Link ID: {}      ", link_id);
            PENGINE_CORE_INFO("|        StartAttr: {}    ", start_attr);
            PENGINE_CORE_INFO("|        EndAttr: {}      ", end_attr);
            PENGINE_CORE_INFO("#################################");
        }

        int destroyed_link_id;
        if (ImNodes::IsLinkDestroyed(&destroyed_link_id))
        {
            // remova da sua lista de conexões
            graph_.connections.erase(
                std::remove_if(graph_.connections.begin(), graph_.connections.end(),
                               [&](auto& c) { return c.id == destroyed_link_id; }),
                graph_.connections.end()
            );
        }
        for (auto& conn : graph_.connections)
        {
            // find the output node
            auto [srcNode, srcIdx] = attrToNodeAttr[conn.start_attr];
            PENGINE_CORE_INFO("SrcIndex: {0}", srcIdx);
            PENGINE_CORE_INFO("Connection start attr: {0}", conn.start_attr);
            PENGINE_CORE_INFO("Connection end attr: {0}", conn.end_attr);
            PENGINE_CORE_INFO("Amount of connections: {0}", attrToNodeAttr.size());
            // find the input node
            auto [dstNode, dstIdx] = attrToNodeAttr[conn.end_attr];
            PENGINE_CORE_INFO("DstIndex: {0}", dstIdx);
            // propagates the value
            dstNode->inputs[dstIdx] = srcNode->outputs[srcIdx];
        }

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

        PrintAllConnectionValues(attrToNodeAttr, graph_.connections);
    }

    void NodeEditor::PrintAllConnectionValues(
        const AttrMap& attrToNodeAttr,
        const std::vector<Connection>& connections
    )
    {
        for (const auto& conn : connections)
        {
            // localiza nó de saída e índice
            auto it_src = attrToNodeAttr.find(conn.start_attr);
            // localiza nó de entrada e índice
            auto it_dst = attrToNodeAttr.find(conn.end_attr);
            if (it_src == attrToNodeAttr.end() || it_dst == attrToNodeAttr.end())
            {
                std::cerr << "Atributo não encontrado para conexão " << conn.id << "\n";
                continue;
            }
            MaterialNode* srcNode = it_src->second.first;
            int srcIdx = it_src->second.second;
            MaterialNode* dstNode = it_dst->second.first;
            int dstIdx = it_dst->second.second;

            string outVal = srcNode->outputs[srcIdx];
            string inVal = dstNode->inputs[dstIdx];

            std::cout
                << "Link " << conn.id
                << ": Node[" << srcNode->id << "].outputs[" << srcIdx << "] = " << outVal
                << "  →  Node[" << dstNode->id << "].inputs[" << dstIdx << "] = " << inVal
                << "\n";
        }
    }

    void NodeEditor::SetupDemoGraph()
    {
        AddNode(Texture2DNode());
        AddNode(MultiplyNode());
        AddNode(LitMasterNode());
    }

    void NodeEditor::RenderConnections()
    {
        for (auto& conn : graph_.connections)
        {
            ImNodes::Link(conn.id, conn.start_attr, conn.end_attr);
        }
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
