#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace ProEngine {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(spec);
#ifdef __APPLE__
    case RendererAPI::API::Metal:
        return nullptr;
#endif
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
