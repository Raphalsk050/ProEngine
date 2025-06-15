#include "PEPCH.h"
#include "Core/Renderer/UniformBuffer.h"

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace ProEngine
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLUniformBuffer>(size, binding);
#ifdef __APPLE__
        case RendererAPI::API::Metal:
            return nullptr;
#endif
        }

        PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
