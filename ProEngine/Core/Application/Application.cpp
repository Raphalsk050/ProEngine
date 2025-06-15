#include "Application.h"

#include "Core/Time.h"
#ifdef PROENGINE_ENABLE_EDITOR
#include "Core/Editor/Frame/Console.h"
#include "Core/Editor/Frame/FpsInspector.h"
#include "Core/Editor/Frame/MainEditorInterface.h"
#endif
#include "Core/Editor/CommandSystem.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/WindowApplicationEvent.h"

namespace ProEngine
{
    Application* Application::instance_ = nullptr;


    Application::Application(const ApplicationSpecification& specification) : specification_(specification)
    {
        PENGINE_CORE_ASSERT(!instance_, "Application already exists!");
        instance_ = this;

        window_ = Window::Create(WindowProps(specification_.Name, specification_.WindowWidth, specification_.WindowHeight));
        window_->SetEventCallback(PROENGINE_BIND_EVENT_FN(Application::OnEvent));
        main_scene_ = CreateScope<Scene>();

#ifdef PROENGINE_ENABLE_EDITOR
        InitializeEditor();
#endif
    }

    Application::~Application() = default;

    void Application::OnEvent(Event& e)
    {
        PENGINE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(PROENGINE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(PROENGINE_BIND_EVENT_FN(Application::OnWindowResize));
        dispatcher.Dispatch<KeyPressedEvent>(PROENGINE_BIND_EVENT_FN(Application::OnKeyPressed));

        for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it)
        {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        PENGINE_PROFILE_FUNCTION();

        layer_stack_.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        PENGINE_PROFILE_FUNCTION();
        layer->OnDetach();
        layer_stack_.PopLayer(layer);
    }

    void Application::PopOverlay(Layer* layer)
    {
        PENGINE_PROFILE_FUNCTION();
        layer->OnDetach();
        layer_stack_.PopOverlay(layer);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        running_ = false;
        return true;
    }

    Scene* Application::GetActiveScene() const
    {
        return main_scene_.get();
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        PENGINE_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            minimized_ = true;
            return false;
        }

        minimized_ = false;

        return false;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& e)
    {
        switch (e.GetKeyCode())
        {
        case Key::Escape:
            {
                PENGINE_CORE_INFO("Escape key was pressed!");
                Close();
            }
        }

        return false;
    }

    void Application::InitializeEditor()
    {
#ifdef PROENGINE_ENABLE_EDITOR
        imgui_layer_ = new ImGuiLayer();

        main_editor_interface_ = new MainEditorInterface();
        PushLayer(imgui_layer_);
        PushLayer(main_editor_interface_);

        RegisterEngineCommands();
#endif
    }

    // Helper function that register commands in the engine to run something
    void Application::RegisterEngineCommands() const
    {
        CommandSystem::Get().RegisterCommand("exit", [this](const std::vector<std::string>&)
        {
            Get().Close();
        });

        CommandSystem::Get().RegisterCommand("help", [this](const std::vector<std::string>&)
        {
            CommandSystem::Get().PrintRegisteredCommands();
        });

        CommandSystem::Get().RegisterCommand("some_helpful_command", [this](const std::vector<std::string>&)
        {
            PENGINE_CORE_INFO("Some helpful command was pressed!");
        });
    }

    void Application::Close()
    {
        running_ = false;
    }

    void Application::SubmitToMainThread(const std::function<void()>& function)
    {
        std::scoped_lock<std::mutex> lock(main_thread_queue_mutex_);

        main_thread_queue_.emplace_back(function);
    }


    void Application::Run()
    {
        PENGINE_PROFILE_FUNCTION();

        while (running_)
        {
            PENGINE_PROFILE_SCOPE("RunLoop");

            int i = 0;
            for (auto layer : layer_stack_)
            {
#ifdef PROENGINE_DEBUG_LAYERS
                PENGINE_CORE_CRITICAL("Layer N.:{} Added layer name: {}", i, layer->GetName());
#endif
                i++;
            }


            float time = Time::GetTime();
            Timestep timestep = time - last_frame_time_;
            last_frame_time_ = time;

            ExecuteMainThreadQueue();

            if (!minimized_)
            {
                {
                    PENGINE_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer : layer_stack_)
                        layer->OnUpdate(timestep);
                }

#ifdef PROENGINE_ENABLE_EDITOR
                imgui_layer_->Begin();
                {
                    PENGINE_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : layer_stack_)
                    {
                        if (layer)
                            layer->OnImGuiRender();
                    }
                }
                imgui_layer_->End();
#endif
            }

            window_->OnUpdate();
        }
    }

    void Application::ExecuteMainThreadQueue()
    {
        std::scoped_lock<std::mutex> lock(main_thread_queue_mutex_);

        for (auto& func : main_thread_queue_)
            func();

        main_thread_queue_.clear();
    }

    void Application::PushOverlay(Layer* layer)
    {
        PENGINE_PROFILE_FUNCTION();

        layer_stack_.PushOverlay(layer);
        layer->OnAttach();
    }
}
