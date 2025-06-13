#include "Window.h"
#include "GenericGLFWWindow.h"

namespace ProEngine
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#if defined(FENGINE_PLATFORM_APPLE) || defined(FENGINE_PLATFORM_WINDOWS)       \
        || defined(FENGINE_PLATFORM_LINUX)
        return CreateScope<GenericWindow>(props);
#else
        FENGINE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
} // namespace ProEngine
