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

        auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        PENGINE_CORE_INFO("OpenGL Info:");
        PENGINE_CORE_INFO("  Vendor:   {0}",   vendor);
        PENGINE_CORE_INFO("  Renderer: {0}",   renderer);
        PENGINE_CORE_INFO("  Version:  {0}",   version);

        // The engine uses DSA functions (glCreateFramebuffers, glCreateTextures,
        // etc) which are part of OpenGL 4.5. Ensure the driver provides at
        // least this version.
        PENGINE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
                            "Pro-Engine requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        PENGINE_PROFILE_FUNCTION();

        glfwSwapBuffers(window_handle_);
    }
}
