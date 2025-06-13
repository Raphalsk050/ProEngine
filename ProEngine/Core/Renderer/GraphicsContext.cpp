#include "GraphicsContext.h"
#include "PEPCH.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace ProEngine {
  Scope<GraphicsContext> GraphicsContext::Create(void* window)
  {
    switch (Renderer::GetAPI())
    {
      case RendererAPI::API::None:    PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
} // BEngine