#include "Application.h"

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
        //PENGINE_PROFILE_FUNCTION();

        layer_stack_.PushLayer(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        running_ = false;
        return true;
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
                Application::Close();
            }
        }

        return false;
    }

    void Application::Close()
    {
        running_ = false;
    }

    void Application::PushOverlay(Layer* layer)
    {
        //FENGINE_PROFILE_FUNCTION();

        layer_stack_.PushOverlay(layer);
        layer->OnAttach();
    }
}
