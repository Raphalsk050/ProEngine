#include "SampleLayer.h"

#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"
#include "Core/Scene/EntityHandle.h"

namespace ProEngine
{
    SampleLayer::SampleLayer() : Layer("SampleLayer")
    {
        auto height = Application::Get().GetWindow().GetHeight();
        auto width = Application::Get().GetWindow().GetWidth();
        camera_controller_ = Camera3DController(width / height);
    }

    void SampleLayer::OnAttach()
    {
        Layer::OnAttach();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        camera_controller_.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        camera_controller_.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

        // TODO(rafael): pass this viewport logic to the editor renderer
        FramebufferSpecification spec;
        spec.Width = Application::Get().GetWindow().GetWidth();
        spec.Height = Application::Get().GetWindow().GetHeight();
        // framebuffer_ = Framebuffer::Create(spec);
        viewport_size_ = {(float)spec.Width, (float)spec.Height};
        camera_controller_.OnResize(spec.Width, spec.Height);
    }

    void SampleLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void SampleLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        camera_controller_.OnUpdate(ts);
        time_ += ts;
        glm::vec3 position = glm::vec3(0.0f, 2.0f, 0.0f);
        //
        // // TODO(rafael): pass this viewport logic to the editor renderer
        RenderCommand::SetClearColor({0.5, 0.01, 0.01, 1.0f});
        RenderCommand::Clear();
        // framebuffer_->Bind();
        //
        Renderer3D::BeginScene(camera_controller_.GetCamera());
        //
        // Renderer3D::DrawSphere(position, 0.2f, glm::vec4(1.0), 0);
        // Renderer3D::SetAmbientLight(glm::vec3(1.0f), 0.2);
        // Renderer3D::DrawMesh(glm::vec3(0.0f, -2.0, 0.0), glm::vec3(100.0f), glm::vec3(0.0f), Mesh::CreatePlane(), glm::vec4(1.0), 1);
        //
        Renderer3D::EndScene();
        // framebuffer_->Unbind();
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
