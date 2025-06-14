#pragma once
#include <imgui.h>
#include "Core/Layer/Layer.h"

namespace ProEngine
{
    class FpsInspector;
    class Console;

    class MainEditorInterface : public Layer
    {
    public:
        MainEditorInterface();
        ~MainEditorInterface() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

    private:
        ImGuiWindowFlags window_flags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiIO* io_;
        void ConfigureDockSpace();
        void RenderTopMenuOptions();
        void OpenConsole();

        void OpenFPSHistory();

        Console* console_;
        FpsInspector* fps_inspector_;

    };
}
