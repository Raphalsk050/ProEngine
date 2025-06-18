#include "Viewport.h"
#include "Core/Renderer/Renderer3D.h"

namespace ProEngine
{
    Viewport::Viewport(HierarchyInspector* hierarchy_inspector) : Layer("Viewport"), hierarchy_inspector_(hierarchy_inspector)
    {
        auto& window = Application::Get().GetWindow();
        camera_controller_ = Camera3DController(window.GetWidth() / (float)window.GetHeight());
    }

    Viewport::~Viewport()
    {
    }

    void Viewport::OnAttach()
    {
        Layer::OnAttach();

        FramebufferSpecification spec;
        spec.Width = Application::Get().GetWindow().GetWidth();
        spec.Height = Application::Get().GetWindow().GetHeight();
        framebuffer_ = Framebuffer::Create(spec);
        window_size_ = {(float)spec.Width, (float)spec.Height};
        camera_controller_.OnResize(spec.Width, spec.Height);
    }

    void Viewport::OnDetach()
    {
        Layer::OnDetach();
    }

    void Viewport::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        camera_controller_.OnUpdate(ts);
        time_ += ts;
        RenderCommand::SetClearColor({0.1f, 0.2f, 0.2f, 1.0f});

        framebuffer_->Bind();
        RenderCommand::Clear();
        Renderer3D::BeginScene(camera_controller_.GetCamera());
        Renderer3D::SetAmbientLight(glm::vec3(1.0f), 10.0);

        // Let the active scene render its entities
        Application::Get().GetActiveScene()->OnUpdate(ts);
        Renderer3D::EndScene();
        framebuffer_->Unbind();
    }

    void Viewport::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        if (ImGui::Begin("Viewport", &opened_))
        {
            ImVec2 size = ImGui::GetContentRegionAvail();
            if (size.x > 0 && size.y > 0 && (size.x != window_size_.x || size.y != window_size_.y))
            {
                framebuffer_->Resize((uint32_t)size.x, (uint32_t)size.y);
                camera_controller_.OnResize(size.x, size.y);
                window_size_ = {size.x, size.y};
            }
            ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), size, ImVec2{0, 1}, ImVec2{1, 0});

            ImVec2 viewportPanelPos = ImGui::GetWindowPos();
            ImVec2 cursor_pos        = ImGui::GetItemRectMin();
            ImVec2 panel_size        = size;

            viewport_location_ = ImGui::GetWindowPos();
            viewport_size_ = ImGui::GetWindowSize();

            ImGuizmo::BeginFrame();
            static ImGuizmo::OPERATION current_gizmo_operation(ImGuizmo::TRANSLATE);
            static ImGuizmo::MODE current_gizmo_mode(ImGuizmo::LOCAL);
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(cursor_pos.x, cursor_pos.y, panel_size.x, panel_size.y);

            auto selected_entity_transform = &hierarchy_inspector_->GetSelectedEntityHandle()->GetComponent<TransformComponent>();
            glm::mat4 transform = selected_entity_transform->LocalMatrix();

            ImGuizmo::Manipulate(
                glm::value_ptr(GetCamera()->GetViewMatrix()),
                glm::value_ptr(GetCamera()->GetProjectionMatrix()),
                current_gizmo_operation,  // ou ROTATE / SCALE
                current_gizmo_mode,                  // ou WORLD
                glm::value_ptr(transform)
            );

            if (ImGuizmo::IsUsing()) {

                selected_entity_transform->position = glm::vec3(transform[3]);
            }


            ImGui::End();
        }
    }

    void Viewport::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        if (camera_movement_enabled_)
        {
            camera_controller_.OnEvent(event);
        }

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(OnKeyReleased));
    }

    void Viewport::Open()
    {
        opened_ = true;
    }

    void Viewport::Close()
    {
        opened_ = false;
    }

    void Viewport::ToggleWindow()
    {
        opened_ = !opened_;
    }

    bool Viewport::OnKeyPressed(KeyPressedEvent& e)
    {
        bool pressed = (e.IsRepeat() || !e.IsRepeat());
        auto KeyCode = e.GetKeyCode();

        if (!last_key_state_map_.contains(KeyCode))
            last_key_state_map_[KeyCode] = false;


        switch (KeyCode)
        {
        case Key::Space:
            {
                if (!last_key_state_map_[KeyCode])
                {
                    last_key_state_map_[KeyCode] = true;
                }

                break;
            }

        case Key::Tab:
            {
                if (!last_key_state_map_[KeyCode])
                {
                    camera_movement_enabled_ = !camera_movement_enabled_;
                    last_key_state_map_[KeyCode] = true;
                    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
                    if (camera_movement_enabled_)
                    {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    }
                    else
                    {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                }

                break;
            }

        case Key::I:
            {
                if (!last_key_state_map_[KeyCode])
                {
                    last_key_state_map_[KeyCode] = true;
                }

                break;
            }
        }

        return false;
    }

    bool Viewport::OnKeyReleased(KeyReleasedEvent& e)
    {
        auto KeyCode = e.GetKeyCode();

        switch (KeyCode)
        {
        case Key::Space:
            {
                last_key_state_map_[KeyCode] = false;
                break;
            }

        case Key::Tab:
            {
                last_key_state_map_[KeyCode] = false;
                break;
            }

        case Key::I:
            {
                last_key_state_map_[KeyCode] = false;
                break;
            }
        }


        return false;
    }
} // ProEngine
