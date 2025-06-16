#include "SceneLayer.h"

#include "imgui.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer3D.h"

namespace ProEngine
{
    SceneLayer::SceneLayer() : Layer("SceneLayer")
    {
        auto& window = Application::Get().GetWindow();
        camera_controller_ = Camera3DController(window.GetWidth() / (float)window.GetHeight());
    }

    void SceneLayer::OnAttach()
    {
        Layer::OnAttach();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        camera_controller_.SetPosition({0.0f, 0.0f, 0.0f});
        camera_controller_.SetRotation({0.0f, 0.0f, 0.0f});

        FramebufferSpecification spec;
        spec.Width = Application::Get().GetWindow().GetWidth();
        spec.Height = Application::Get().GetWindow().GetHeight();
        framebuffer_ = Framebuffer::Create(spec);
        viewport_size_ = {(float)spec.Width, (float)spec.Height};
        camera_controller_.OnResize(spec.Width, spec.Height);
        Renderer3D::EnableWireframe(true);

        // Create an example entity with renderer component
        auto* scene = Application::Get().GetActiveScene();
        cube_entity_ = scene->CreateEntity("Cube Entity");
        auto& renderer = cube_entity_.AddComponent<RendererComponent>();
        renderer.mesh = MeshType::Cube;
        renderer.color = glm::vec4(0.8f, 0.8f, 0.1f, 1.0f);
    }

    void SceneLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        camera_controller_.OnUpdate(ts);
        time_ += ts;
        RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        framebuffer_->Bind();
        RenderCommand::Clear();
        Renderer3D::BeginScene(camera_controller_.GetCamera());
        Renderer3D::SetAmbientLight(glm::vec3(1.0f), 0.2);

        // Let the active scene render its entities
        Application::Get().GetActiveScene()->OnUpdate(ts);

        Renderer3D::EndScene();
        framebuffer_->Unbind();
    }

    void SceneLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        ImGui::Begin("Viewport");
        ImVec2 size = ImGui::GetContentRegionAvail();
        if (size.x > 0 && size.y > 0 && (size.x != viewport_size_.x || size.y != viewport_size_.y))
        {
            framebuffer_->Resize((uint32_t)size.x, (uint32_t)size.y);
            camera_controller_.OnResize(size.x, size.y);
            viewport_size_ = {size.x, size.y};
        }
        ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), size, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::End();
    }

    void SceneLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    bool SceneLayer::OnKeyPressed(KeyPressedEvent& e)
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

    bool SceneLayer::OnKeyReleased(KeyReleasedEvent& e)
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

    void SceneLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
        if (camera_movement_enabled_)
        {
            camera_controller_.OnEvent(e);
        }

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(PENGINE_BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(PENGINE_BIND_EVENT_FN(OnKeyReleased));
    }
} // namespace ProEngine
