#pragma once
#include <imgui.h>
#include <chrono>
#include <string>
#include <cmath>

namespace ProEngine
{
    class PopupWithoutOverlay
    {
    public:
        void Open()
        {
            is_open_ = true;
            start_time_ = std::chrono::steady_clock::now();
        }

        void Close()
        {
            is_open_ = false;
        }

        void Draw(const char* title, const char* message, ImVec2 center = ImVec2(0.0f, 0.0f))
        {
            if (!is_open_) return;

            // Calcula o progresso da animação (0.0 a 1.0)
            auto centerX = center.x;
            auto centerY = center.y;
            auto now = std::chrono::steady_clock::now();
            float time_elapsed = std::chrono::duration<float>(now - start_time_).count();
            float progress = std::min(time_elapsed / animation_duration_, 1.0f);

            // Aplica easing (ease-out cubic)
            progress = 1.0f - std::pow(1.0f - progress, 3.0f);

            // Configura o popup no centro da tela
            ImVec2 screen_size = ImGui::GetIO().DisplaySize;
            float right_offset = (1.0f - progress) * (screen_size.x * 0.5f - ImGui::GetWindowViewport()->GetWorkSize().x * 0.5f);

            center = ImVec2(0.0f, screen_size.y * 0.8f);
            //center = ImVec2(centerX, centerY);
            ImGui::SetNextWindowPos(ImVec2(right_offset, center.y), ImGuiCond_Always, ImVec2(1.0f, 0.5f));

            // Animação de scale e fade
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, progress);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.95f));

            // Scale effect - movimento de baixo para cima
            float y_offset = (1.0f - progress) * 200.0f;
            ImGui::SetNextWindowPos(ImVec2(right_offset, center.y), ImGuiCond_Always, ImVec2(-2.0f, 0.5f));

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_AlwaysAutoResize;

            if (ImGui::Begin(title, nullptr, flags))
            {
                ImGui::Text("%s", message);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                float button_width = 100.0f;
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - button_width) * 0.5f);

                if (ImGui::Button("OK", ImVec2(button_width, 30)))
                {
                    Close();
                }

                ImGui::End();
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleVar(2);
        }

        bool IsOpen() const { return is_open_; }

    private:
        bool is_open_ = false;
        std::chrono::steady_clock::time_point start_time_;
        float animation_duration_ = 0.3f;
    };

    // ========== VERSÃO 2: COM OVERLAY CLICÁVEL ==========
    class PopupWithOverlay
    {
    public:
        void Open()
        {
            is_open_ = true;
            start_time_ = std::chrono::steady_clock::now();
        }

        void Close()
        {
            is_open_ = false;
        }

        void Draw(const char* title, const char* message)
        {
            if (!is_open_) return;

            // Calcula o progresso da animação
            auto now = std::chrono::steady_clock::now();
            float time_elapsed = std::chrono::duration<float>(now - start_time_).count();
            float progress = std::min(time_elapsed / animation_duration_, 1.0f);
            progress = 1.0f - std::pow(1.0f - progress, 3.0f);

            ImVec2 screen_size = ImGui::GetIO().DisplaySize;

            // Overlay INVISÍVEL mas clicável
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(screen_size);
            ImGui::SetNextWindowBgAlpha(0.3f * progress); // Semi-transparente

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            if (ImGui::Begin("##Overlay", nullptr,
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoSavedSettings))
            {
                // Detecta clique no overlay para fechar
                if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
                {
                    ImVec2 mouse_pos = ImGui::GetMousePos();
                    ImVec2 window_pos, window_size;

                    // Verifica se o clique foi fora do popup
                    // (isso será calculado abaixo)
                    should_close_on_overlay_click_ = true;
                }

                ImGui::End();
            }

            ImGui::PopStyleVar(3);

            // Popup principal
            ImVec2 center = ImVec2(screen_size.x * 0.5f, screen_size.y * 0.5f);
            float y_offset = (1.0f - progress) * 30.0f;
            ImGui::SetNextWindowPos(ImVec2(center.x, center.y + y_offset), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, progress);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

            if (ImGui::Begin(title, nullptr,
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_AlwaysAutoResize))
            {
                // Se clicou no popup, não fecha
                if (ImGui::IsWindowHovered())
                {
                    should_close_on_overlay_click_ = false;
                }

                ImGui::Text("%s", message);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Button("OK", ImVec2(100, 30)))
                {
                    Close();
                }

                ImGui::End();
            }

            ImGui::PopStyleVar(2);

            // Fecha se clicou no overlay (fora do popup)
            if (should_close_on_overlay_click_)
            {
                Close();
                should_close_on_overlay_click_ = false;
            }
        }

        bool IsOpen() const { return is_open_; }

    private:
        bool is_open_ = false;
        bool should_close_on_overlay_click_ = false;
        std::chrono::steady_clock::time_point start_time_;
        float animation_duration_ = 0.3f;
    };

    // ========== VERSÃO 3: POPUP MODAL IMGUI NATIVO ==========
    class ModalPopup
    {
        bool should_open_ = false;
        float alpha_ = 0.0f;

    public:
        void Open()
        {
            should_open_ = true;
        }

        void Draw(const char* title, const char* message)
        {
            if (should_open_)
            {
                ImGui::OpenPopup(title);
                should_open_ = false;
                alpha_ = 0.0f;
            }

            // Animação de fade
            if (ImGui::IsPopupOpen(title))
            {
                alpha_ = std::min(alpha_ + ImGui::GetIO().DeltaTime * 4.0f, 1.0f);
            }

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha_);

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("%s", message);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Button("OK", ImVec2(120, 0)))
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::PopStyleVar();
        }
    };

    class SimplestPopup
    {
        float alpha_ = 0.0f;
        bool is_open_ = false;

    public:
        void Toggle() { is_open_ = !is_open_; }
        void Open() { is_open_ = true; }
        void Close() { is_open_ = false; }

        void Draw()
        {
            // Atualiza alpha suavemente
            float target_alpha = is_open_ ? 1.0f : 0.0f;
            alpha_ += (target_alpha - alpha_) * ImGui::GetIO().DeltaTime * 10.0f;

            if (alpha_ < 0.01f) return;

            // Centraliza e desenha
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha_);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

            if (ImGui::Begin("Popup", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse))
            {
                ImGui::Text("Este é um popup simples!");
                ImGui::Text("Alpha: %.2f", alpha_);
                ImGui::Spacing();

                if (ImGui::Button("Fechar", ImVec2(100, 30)))
                {
                    Close();
                }

                ImGui::End();
            }

            ImGui::PopStyleVar(2);
        }
    };
}
