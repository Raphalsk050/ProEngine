#pragma once
#include "imgui.h"
#include "Core/Layer/Layer.h"
#include "Core/Log/PELog.h"
#include "spdlog/sinks/ringbuffer_sink.h"

namespace ProEngine
{
    class Console : public Layer
    {
    public:
        Console();
        ~Console() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

        void OpenConsole();
        void CloseConsole();

    private:
        bool open_;
        ImVec2 console_size_;
        ImFont* console_font_;
        std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> log_sink_;
        ImGuiWindowFlags window_flags_ = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar;
        void RenderConsoleOutputWindow();
        void RenderConsoleInputWindow();
    };
} // ProEngine
