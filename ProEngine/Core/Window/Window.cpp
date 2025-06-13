#include "Window.h"
#include "GenericGLFWWindow.h"

namespace ProEngine
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#if defined(PROENGINE_PLATFORM_APPLE) || defined(PROENGINE_PLATFORM_WINDOWS)       \
        || defined(PROENGINE_PLATFORM_LINUX)
        return CreateScope<GenericWindow>(props);
#else
        PENGINE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
} // namespace ProEngine
