#include "Core/Editor/ImguiLayer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

#include "PEPCH.h"
#include "Core/Application/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
//#include <ImGuizmo.h>
#include <glad/glad.h>

#include "Core/Renderer/RenderCommand.h"

namespace ProEngine
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {
    }

    void ImGuiLayer::OnAttach()
    {
        PENGINE_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF("../ProEngine/Assets/Fonts/PT-Root-UI/pt-root-ui_regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("../ProEngine/Assets/Fonts/Inter/Inter-Regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("../ProEngine/Assets/Fonts/Nunito/Nunito-Regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("../ProEngine/Assets/Fonts/Raleway/Raleway-Regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("../ProEngine/Assets/Fonts/JetBrains/JetBrainsMonoNerdFont-Regular.ttf", 16.0f);

        //io.Fonts->AddFontFromFileTTF(font_path_, 18.0f);

        //io.Fonts->Build();

        (void)io;
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport

        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window =
            static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void ImGuiLayer::OnDetach()
    {
        PENGINE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        if (block_events_)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::ShowMetricsWindow()
    {
        const float DISTANCE = 10.0f;
        static int corner = 0;
        ImGuiIO& io = ImGui::GetIO();
        if (corner != -1)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_area_pos =
                viewport->GetWorkPos(); // Instead of using viewport->Pos we use
            // GetWorkPos() to avoid menu bars, if any!
            ImVec2 work_area_size = viewport->GetWorkSize();
            ImVec2 window_pos =
                ImVec2((corner & 1)
                           ? (work_area_pos.x + work_area_size.x - DISTANCE)
                           : (work_area_pos.x + DISTANCE),
                       (corner & 2)
                           ? (work_area_pos.y + work_area_size.y - DISTANCE)
                           : (work_area_pos.y + DISTANCE));
            ImVec2 window_pos_pivot =
                ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1) window_flags |= ImGuiWindowFlags_NoMove;
        if (ImGui::Begin("Forge Engine Status", (bool*)1, window_flags))
        {
            ImGui::Text("Forge Engine Status\n\n\n\n");
            ImGui::Separator();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Separator();
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse Position: <invalid>");
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
                if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
                if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                ImGui::EndPopup();
            }

            /*auto stats = PENGINE::Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());*/
        }
        ImGui::End();
    }

    void ImGuiLayer::Begin()
    {
        PENGINE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //ImGuizmo::BeginFrame();
        ImGui::ShowDemoWindow();
        //ShowMetricsWindow();
    }

    void ImGuiLayer::End()
    {
        PENGINE_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(),
                                (float)app.GetWindow().GetHeight());
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    }

    uint32_t ImGuiLayer::GetActiveWidgetID() const { return GImGui->ActiveId; }
} // namespace PENGINE
