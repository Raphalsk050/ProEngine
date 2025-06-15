#include "PEPCH.h"
#include "Core/Renderer/VertexArray.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ProEngine {

    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexArray>();
#ifdef __APPLE__
        case RendererAPI::API::Metal:
            return nullptr;
#endif
        }

        PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}