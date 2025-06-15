#include "RendererAPI.h"

#include "Config.h"
#include "RendererAPI.h"
#include "PEPCH.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#ifdef __APPLE__
#include "Platform/Metal/MetalRendererAPI.h"
#endif

namespace ProEngine {
#ifdef __APPLE__
    RendererAPI::API RendererAPI::api_ = API::Metal;
#else
    RendererAPI::API RendererAPI::api_ = API::OpenGL;
#endif

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (api_)
        {
        case RendererAPI::API::None:    PENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
#ifdef __APPLE__
        case RendererAPI::API::Metal:   return CreateScope<MetalRendererAPI>();
#endif
       }

        PENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // BEngine