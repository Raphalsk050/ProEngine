#pragma once
#include <string>
#include <functional>
#include <mutex>
#include "PEPCH.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/WindowApplicationEvent.h"
#include "Core/Editor/ImguiLayer.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Scene/Scene.h"
#include "Core/Window/GenericGLFWWindow.h"

namespace ProEngine
{
    class MainEditorInterface;
}

namespace ProEngine
{
    class FpsInspector;
}

using namespace std;

namespace ProEngine
{
    class Console;

    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            PENGINE_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    struct ApplicationSpecification
    {
        string Name = "Pro-Engine Application";
        string WorkingDirectory;
        uint32_t WindowWidth = 1280;
        uint32_t WindowHeight = 720;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification);

        ~Application();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        bool OnWindowClose(WindowCloseEvent& e);

        Window& GetWindow() const { return *window_; }
        static Application& Get() { return *instance_; }
        Scene* GetActiveScene() const;

        bool OnWindowResize(WindowResizeEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        void InitializeEditor();
        void Close();
        void SubmitToMainThread(const std::function<void()>& function);
        void Run();
        void ExecuteMainThreadQueue();

    private:
        ApplicationSpecification specification_;

    private:
        void RegisterEngineCommands() const;

    private:
        LayerStack layer_stack_;
        Scope<Window> window_;

    private:
        // Editor
        ImGuiLayer* imgui_layer_{};
        Console* console_layer_{};
        FpsInspector* fps_inspector_layer_{};
        MainEditorInterface* main_editor_interface_;

        bool running_ = true;
        bool minimized_ = false;
        float last_frame_time_ = 0.0f;
        Scope<Scene> main_scene_;
        vector<function<void()>> main_thread_queue_;
        mutex main_thread_queue_mutex_;

    private:
        static Application* instance_;
    };

    Application* CreateApplication(ApplicationCommandLineArgs args);
}
