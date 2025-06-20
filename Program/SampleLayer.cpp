#include "SampleLayer.h"

#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"
#include "Core/Scene/EntityHandle.h"
#include "Core/Camera/FilamentCameraController.h"

namespace ProEngine
{
    SampleLayer::SampleLayer() : Layer("SampleLayer")
    {
        auto height = Application::Get().GetWindow().GetHeight();
        auto width = Application::Get().GetWindow().GetWidth();
        Renderer3D::Init(width, height);
        camera_controller_ = FilamentCameraController(Renderer3D::GetEngine(), width / height);
    }

    void SampleLayer::OnAttach()
    {
        Layer::OnAttach();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void SampleLayer::OnDetach()
    {
        Layer::OnDetach();
        Renderer3D::Shutdown();
    }

    void SampleLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        camera_controller_.OnUpdate(ts);
        time_ += ts;
        RenderCommand::SetClearColor({0.5, 0.01, 0.01, 1.0f});
        RenderCommand::Clear();

        Renderer3D::BeginScene(camera_controller_.GetCamera());

        Renderer3D::EndScene();
    }

    void SampleLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        // static PopupWithoutOverlay popup_without_overlay;
        // static PopupWithOverlay popup_with_overlay;
        // static ModalPopup modal_popup;
        // static SimplestPopup simplest_popup;
        //
        // if (ImGui::Begin("Teste de Popups"))
        // {
        //     ImGui::Text("Escolha o tipo de popup:");
        //     ImGui::Separator();
        //
        //     if (ImGui::Button("Popup Sem Overlay", ImVec2(200, 30)))
        //     {
        //         popup_without_overlay.Open();
        //     }
        //
        //     if (ImGui::Button("Popup Com Overlay Clicável", ImVec2(200, 30)))
        //     {
        //         popup_with_overlay.Open();
        //     }
        //
        //     if (ImGui::Button("Popup Modal Nativo", ImVec2(200, 30)))
        //     {
        //         modal_popup.Open();
        //     }
        //
        //     if (ImGui::Button("Popup Mais Simples", ImVec2(200, 30)))
        //     {
        //         simplest_popup.Toggle();
        //     }
        // }
        // ImGui::End();
        //
        // // Desenha os popups
        // popup_without_overlay.Draw("Aviso", "Este é um popup sem overlay!\nVocê ainda pode interagir com o resto da interface.");
        // popup_with_overlay.Draw("Informação", "Este popup tem overlay.\nClique fora para fechar.");
        // modal_popup.Draw("Confirmação", "Deseja continuar com a operação?");
        // simplest_popup.Draw();
    }

    void SampleLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }
} // ProEngine
