#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#ifdef PROENGINE_USE_FILAMENT_FRAMEBUFFER
#include "Platform/Filament/FilamentFramebuffer.h"
#endif

namespace ProEngine {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    if (spec.UseFilament)
    {
#ifdef PROENGINE_USE_FILAMENT_FRAMEBUFFER
        return CreateRef<FilamentFramebuffer>(spec);
#else
        PENGINE_CORE_WARN("Filament framebuffer requested but support not enabled");
#endif
    }

    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(spec);
    }

    PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
