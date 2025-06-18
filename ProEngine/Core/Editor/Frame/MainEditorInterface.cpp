#include "MainEditorInterface.h"

#include <iostream>

#include "Console.h"
#include "FpsInspector.h"
#include "ImGuizmo.h"
#include "Core/Application/Application.h"
#include "gtc/type_ptr.hpp"
#include "Hierarchy/HierarchyInspector.h"

namespace ProEngine
{
    MainEditorInterface::MainEditorInterface(): io_(nullptr), console_(nullptr), fps_inspector_(nullptr)
    {
        debug_name_ = "MainEditorLayer";
    }

    MainEditorInterface::~MainEditorInterface()
    = default;

    void MainEditorInterface::OnAttach()
    {
        Layer::OnAttach();
        io_ = &ImGui::GetIO();

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // creates the main engine ui. This creates a UI docker space to attach other ui layers.
        window_flags_ |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags_ |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags_ |= ImGuiWindowFlags_NoBackground;
        window_flags_ |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        InitializeFrames();
    }

    void MainEditorInterface::OnDetach()
    {
        Layer::OnDetach();
    }

    void MainEditorInterface::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void MainEditorInterface::OnImGuiRender()
    {
        Layer::OnImGuiRender();

        if (!io_)
        {
            return;
        }

        ConfigureDockSpace();
    }

    void MainEditorInterface::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void MainEditorInterface::InitializeFrames()
    {
        // layers initialization
        hierarchy_inspector_ = new HierarchyInspector();
        fps_inspector_ = new FpsInspector();
        viewport_inspector_ = new Viewport(hierarchy_inspector_);
        console_ = new Console();
        node_editor_ = new NodeEditor();

        // push layers
        Application::Get().PushLayer(hierarchy_inspector_);
        Application::Get().PushLayer(fps_inspector_);
        Application::Get().PushLayer(viewport_inspector_);
        Application::Get().PushLayer(console_);
        Application::Get().PushLayer(node_editor_);
    }

    void MainEditorInterface::ConfigureDockSpace()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        bool dockspace_open_dummy = true;
        ImGui::Begin("MainEngineDockspace", &dockspace_open_dummy, window_flags_);

        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("PrimaryDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        RenderTopMenuOptions();

        ImGui::End();
    }

    void MainEditorInterface::RenderTopMenuOptions()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Console"))
                {
                    ToggleConsole();
                }

                if (ImGui::MenuItem("FPS History"))
                {
                    ToggleFPSHistory();
                }

                if (ImGui::MenuItem("Hierarchy"))
                {
                    ToggleHierarchyInspector();
                }
                if (ImGui::MenuItem("Viewport"))
                {
                    ToggleViewport();
                }
                if (ImGui::MenuItem("Node Editor"))
                {
                    ToggleNodeEditor();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void MainEditorInterface::ToggleConsole()
    {
        if (!console_)
        {
            console_ = new Console();
            Application::Get().PushLayer(console_);
        }

        console_->ToggleWindow();
    }

    // TODO(rafael): create a window generic class to be the parent of these kind of layers
    void MainEditorInterface::ToggleHierarchyInspector()
    {
        if (!hierarchy_inspector_)
        {
            hierarchy_inspector_ = new HierarchyInspector();
            Application::Get().PushLayer(hierarchy_inspector_);
        }

        hierarchy_inspector_->ToggleWindow();
    }

    void MainEditorInterface::ToggleFPSHistory()
    {
        if (!fps_inspector_)
        {
            fps_inspector_ = new FpsInspector();
            Application::Get().PushLayer(fps_inspector_);
        }

        fps_inspector_->ToggleWindow();
    }

    void MainEditorInterface::ToggleViewport()
    {
        if (!viewport_inspector_)
        {
            viewport_inspector_ = new Viewport(hierarchy_inspector_);
            Application::Get().PushLayer(viewport_inspector_);
        }

        viewport_inspector_->ToggleWindow();
    }

    void MainEditorInterface::ToggleNodeEditor()
    {
        if (!node_editor_)
        {
            node_editor_ = new NodeEditor();
            Application::Get().PushLayer(node_editor_);
        }

        node_editor_->ToggleWindow();
    }
}
