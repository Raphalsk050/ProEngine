#include "Core/Renderer/Texture.h"

#include "PEPCH.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Core/Renderer/Renderer.h"

namespace ProEngine
{
    Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            PENGINE_CORE_ASSERT(false,
                                "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(specification);
#ifdef __APPLE__
        case RendererAPI::API::Metal:
            return nullptr;
#endif
        }

        PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            PENGINE_CORE_ASSERT(false,
                                "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(path);
#ifdef __APPLE__
        case RendererAPI::API::Metal:
            return nullptr;
#endif
        }

        PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace BEngine
