#pragma once
#include <glm.hpp>
#include <imgui.h>

#include "gtc/type_ptr.hpp"

namespace ProEngine
{
    class Vector3Field
    {
    public:
        static void RenderField(glm::fvec3* vector, int id = 0)
        {
            float default_field_size_ = 50.0f;
            ImGui::BeginChild("##transform_child", ImVec2(0, 0), false);

            auto id_str = "##" + std::to_string(id);
            ImGui::AlignTextToFramePadding();
            ImGui::TextColored(ImVec4(1,0,0,1), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->x, 0.01f, 0.0f, 0.0f, "%.2f");

            id++;
            id_str = "##" + std::to_string(id);
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::TextColored(ImVec4(0,1,0,1), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->y, 0.01f, 0.0f, 0.0f, "%.2f");

            id++;
            id_str = "##" + std::to_string(id);
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::TextColored(ImVec4(0,0,1,1), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->z, 0.01f, 0.0f, 0.0f, "%.2f");

            ImGui::EndChild();
        }
    };
}
