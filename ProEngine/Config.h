#pragma once
#include <memory>

// this flag serves to show the shader's possible compilation errors and warnings
// #define PROENGINE_SHADER_DEBUG

// to controls the render debug
// #define PROENGINE_RENDER_DEBUG

// to enable the culling debug
// #define PROENGINE_CULLING_DEBUG

// to debug the frustum information
// #define PROENGINE_DEBUG_FRUSTUM

// to debug key input
// #define PROENGINE_DEBUG_INPUT_KEYS

// to enable the runtime editor
// #define PROENGINE_ENABLE_EDITOR

// to debug the layers
// #define PROENGINE_DEBUG_LAYERS

#ifdef DEBUG
#if defined(PROENGINE_PLATFORM_WINDOWS)
    #define PROENGINE_DEBUGBREAK() __debugbreak()
#elif defined(PROENGINE_PLATFORM_LINUX)
    #include <signal.h>
    #define PROENGINE_DEBUGBREAK() raise(SIGTRAP)
#elif defined(PROENGINE_PLATFORM_APPLE)
#include <signal.h>
#define PROENGINE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#else
#define PROENGINE_DEBUGBREAK()
#endif

#define PROENGINE_EXPAND_MACRO(x) x
#define PROENGINE_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define PROENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace ProEngine
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#include "Core/Log/PELog.h"
#include "Core/Assert/Assert.h"
