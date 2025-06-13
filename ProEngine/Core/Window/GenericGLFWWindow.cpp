#include "GenericGLFWWindow.h"
#include "Core/Event/Event.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowApplicationEvent.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RendererAPI.h"
#include "FEPCH.h"
#include "glad/glad.h"

namespace ForgeEngine {

    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        FENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    GenericWindow::GenericWindow(const WindowProps& props)
    {
        FENGINE_PROFILE_FUNCTION();

        GenericWindow::Init(props);
    }

    GenericWindow::~GenericWindow()
    {
        FENGINE_PROFILE_FUNCTION();

        Shutdown();
    }

    void GenericWindow::Init(const WindowProps& props)
    {
        FENGINE_PROFILE_FUNCTION();

        data_.Title = props.Title;
        data_.Width = props.Width;
        data_.Height = props.Height;

        FENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title,
                          props.Width, props.Height);

        if (s_GLFWWindowCount == 0) {
            FENGINE_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            FENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            FENGINE_PROFILE_SCOPE("glfwCreateWindow");
#if defined(DEBUG)
            if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            window_ = glfwCreateWindow((int)props.Width, (int)props.Height,
                                       data_.Title.c_str(), nullptr, nullptr);
            ++s_GLFWWindowCount;
        }

        context_ = GraphicsContext::Create(window_);
        context_->Init();

        glfwSetWindowUserPointer(window_, &data_);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width,
                                              int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
            FENGINE_CORE_INFO("Window Resized ({},{})", width, height);
            glad_glViewport(0, 0, width, height);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key,
                                       int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, true);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetCharCallback(window_, [](GLFWwindow* window,
                                        unsigned int keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button,
                                               int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset,
                                          double yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos,
                                             double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void GenericWindow::Shutdown()
    {
        FENGINE_PROFILE_FUNCTION();

        glfwDestroyWindow(window_);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0) { glfwTerminate(); }
    }

    void GenericWindow::OnUpdate()
    {
        FENGINE_PROFILE_FUNCTION();

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        context_->SwapBuffers();
    }

    void GenericWindow::SetVSync(bool enabled)
    {
        FENGINE_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        data_.VSync = enabled;
    }

    bool GenericWindow::IsVSync() const
    {
        return data_.VSync;
    }

} // namespace ForgeEngine
