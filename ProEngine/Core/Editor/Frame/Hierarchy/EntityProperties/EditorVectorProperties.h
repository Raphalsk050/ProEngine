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
            int default_field_spacing = 12;

            /////////Vector X Component////////////////////////////////////////////////////
            const char* label = "X";
            ImGui::BeginChild("##transform_child", ImVec2(0, 0), false);

            auto id_str = "##" + std::to_string(id);

            for (int i = 0; i < 5; i++)
                ImGui::Spacing();
            ImGui::SameLine(0, 5);
            ImGui::AlignTextToFramePadding();
            RenderTextBackground(label);
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", label);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->x, 0.01f, 0.0f, 0.0f, "%.2f");
            auto size = ImGui::GetItemRectSize();

            /////////Vector Y Component////////////////////////////////////////////////////
            label = "Y";
            id++;
            id_str = "##" + std::to_string(id);
            ImGui::SameLine(0,default_field_spacing);
            ImGui::AlignTextToFramePadding();

            RenderTextBackground(label);
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", label);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->y, 0.01f, 0.0f, 0.0f, "%.2f");

            /////////Vector Z Component////////////////////////////////////////////////////
            label = "Z";
            id++;
            id_str = "##" + std::to_string(id);
            ImGui::SameLine(0,default_field_spacing);
            ImGui::AlignTextToFramePadding();
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 0, 0, 0));

            RenderTextBackground(label);
            ImGui::TextColored(ImVec4(0, 0, 1, 1), "%s", label);
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::SetNextItemWidth(default_field_size_);
            ImGui::DragFloat(id_str.c_str(), &vector->z, 0.01f, 0.0f, 0.0f, "%.2f");

            ImGui::EndChild();
        }

    private:
        static void RenderTextBackground(const char* label, ImU32 bgColor = IM_COL32(0, 0, 0, 255),float outer_padding = -3.0f)
        {
            float default_field_size = 55.0f;
            //text background color
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 textSize = ImGui::CalcTextSize(label);
            float padX = 6, padY = 2;
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - padX, pos.y + outer_padding),
                                                      ImVec2(pos.x + textSize.x + padX + default_field_size, pos.y + 30.0 - outer_padding), bgColor, 5);
        }
    };
}
