#include "SampleLayer.h"

#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Scene/EntityHandle.h"

namespace ProEngine
{
    SampleLayer::SampleLayer() : Layer("SampleLayer")
    {
    }

    void SampleLayer::OnAttach()
    {
        Layer::OnAttach();
        Application::Get().GetActiveScene()->CreateEntity();

        for (int i = 0; i < 2; i++)
        {
            Application::Get().GetActiveScene()->CreateEntity("Entity(" + std::to_string(i) + ")");
        }
    }

    void SampleLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void SampleLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void SampleLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();

        static PopupWithoutOverlay popup_without_overlay;
        static PopupWithOverlay popup_with_overlay;
        static ModalPopup modal_popup;
        static SimplestPopup simplest_popup;

        if (ImGui::Begin("Teste de Popups"))
        {
            ImGui::Text("Escolha o tipo de popup:");
            ImGui::Separator();

            if (ImGui::Button("Popup Sem Overlay", ImVec2(200, 30)))
            {
                popup_without_overlay.Open();
            }

            if (ImGui::Button("Popup Com Overlay Clicável", ImVec2(200, 30)))
            {
                popup_with_overlay.Open();
            }

            if (ImGui::Button("Popup Modal Nativo", ImVec2(200, 30)))
            {
                modal_popup.Open();
            }

            if (ImGui::Button("Popup Mais Simples", ImVec2(200, 30)))
            {
                simplest_popup.Toggle();
            }
        }
        ImGui::End();

        // Desenha os popups
        popup_without_overlay.Draw("Aviso", "Este é um popup sem overlay!\nVocê ainda pode interagir com o resto da interface.");
        popup_with_overlay.Draw("Informação", "Este popup tem overlay.\nClique fora para fechar.");
        modal_popup.Draw("Confirmação", "Deseja continuar com a operação?");
        simplest_popup.Draw();
    }

    void SampleLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }
} // ProEngine
