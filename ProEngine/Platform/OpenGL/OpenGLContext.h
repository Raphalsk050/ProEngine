#pragma once
#include "Core/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace ProEngine {
  class OpenGLContext : public GraphicsContext {
  public:
    explicit OpenGLContext(GLFWwindow *windowHandle);

    void Init() override;

    void SwapBuffers() override;

  private:
    GLFWwindow *window_handle_;
  };
}
