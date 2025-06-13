#include "OpenGLContext.h"
#include "PEPCH.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace ProEngine
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : window_handle_(windowHandle)
    {
        PENGINE_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::Init()
    {
        PENGINE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(window_handle_);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        PENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");

        PENGINE_CORE_INFO("OpenGL Info:");
        PENGINE_CORE_INFO("  Vendor: {0}", *glGetString(GL_VENDOR));
        PENGINE_CORE_INFO("  Renderer: {0}", *glGetString(GL_RENDERER));
        PENGINE_CORE_INFO("  Version: {0}", *glGetString(GL_VERSION));

        // TODO: Fix this check
        PENGINE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 1),
                        "Pro-Engine requires at least OpenGL version 4.1!");
    }

    void OpenGLContext::SwapBuffers()
    {
        PENGINE_PROFILE_FUNCTION();

        glfwSwapBuffers(window_handle_);
    }
}
