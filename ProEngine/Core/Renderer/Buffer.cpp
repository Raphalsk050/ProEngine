#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ProEngine {
  Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
      case RendererAPI::API::None:
        PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size);
#ifdef __APPLE__
      case RendererAPI::API::Metal:
        return nullptr; // Metal implementation not yet available
#endif
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }

  Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
      case RendererAPI::API::None:
        PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
#ifdef __APPLE__
      case RendererAPI::API::Metal:
        return nullptr;
#endif
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
      case RendererAPI::API::None:
        PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(indices, size);
#ifdef __APPLE__
      case RendererAPI::API::Metal:
        return nullptr;
#endif
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }
} // BEngine
