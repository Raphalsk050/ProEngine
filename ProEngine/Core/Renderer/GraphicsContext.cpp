#include "GraphicsContext.h"
#include "PEPCH.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#ifdef __APPLE__
#include "Platform/Metal/MetalContext.h"
#endif

namespace ProEngine {
  Scope<GraphicsContext> GraphicsContext::Create(void* window)
  {
    switch (Renderer::GetAPI())
    {
      case RendererAPI::API::None:
        PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
#ifdef __APPLE__
      case RendererAPI::API::Metal:
        return CreateScope<MetalContext>(window);
#endif
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
} // BEngine