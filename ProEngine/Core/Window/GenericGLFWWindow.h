#pragma once

#include "Core/Window/Window.h"
#include "Core/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace ProEngine {

  class GenericWindow : public Window
  {
  public:
    GenericWindow(const WindowProps& props);
    virtual ~GenericWindow();

    void OnUpdate() override;

    unsigned int GetWidth() const override { return data_.Width; }
    unsigned int GetHeight() const override { return data_.Height; }

    // Window attributes
    void SetEventCallback(const EventCallbackFn& callback) override { data_.EventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    virtual void* GetNativeWindow() const { return window_; }
  private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();
  private:
    GLFWwindow* window_;
    Scope<GraphicsContext> context_;

    struct WindowData
    {
      std::string Title;
      unsigned int Width, Height;
      bool VSync;

      EventCallbackFn EventCallback;
    };

    WindowData data_;
  };

}