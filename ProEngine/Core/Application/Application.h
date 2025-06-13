#pragma once
#include <string>
#include "PEPCH.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/WindowApplicationEvent.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Window/GenericGLFWWindow.h"

using namespace std;

namespace ProEngine {
  struct ApplicationCommandLineArgs {
    int Count = 0;
    char **Args = nullptr;

    const char *operator[](int index) const {
      PENGINE_CORE_ASSERT(index < Count);
      return Args[index];
    }
  };

  struct ApplicationSpecification {
    string Name = "Pro-Engine Application";
    string WorkingDirectory;
    uint32_t WindowWidth = 1280;
    uint32_t WindowHeight = 720;
    ApplicationCommandLineArgs CommandLineArgs;
  };

  class Application {
  public:
    Application(const ApplicationSpecification &specification);

    ~Application();
    void OnEvent(Event& e);

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *layer);
    bool OnWindowClose(WindowCloseEvent& e);

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
    void Close();

  private:
    ApplicationSpecification specification_;

  private:
    LayerStack layer_stack_;
    Scope<Window> window_;
    bool running_ = true;
    bool minimized_ = false;
    float last_frame_time_ = 0.0f;

  private:
    static Application *instance_;
  };

  Application *CreateApplication(ApplicationCommandLineArgs args);
}
