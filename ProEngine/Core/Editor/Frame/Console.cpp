#include "Console.h"


#include "imgui.h"
#include "Core/Editor/CommandSystem.h"

namespace ProEngine
{
    Console::Console(): opened_(false)
    {
        debug_name_ = "ConsoleLayer";
    }

    Console::~Console()
    {
    }

    void Console::OnAttach()
    {
        Layer::OnAttach();

        log_sink_ = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1000);
        auto coreLogger = PELog::GetCoreLogger();
        if (coreLogger)
        {
            coreLogger->sinks().push_back(log_sink_);
        }
    }

    void Console::OnDetach()
    {
        Layer::OnDetach();
    }

    void Console::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void Console::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        console_size_ = ImGui::GetWindowContentRegionMax();

        if (!opened_) return;
        if (ImGui::Begin("Console", &opened_))
        {
            if (ImGui::Button("Clear"))
            {
                if (log_sink_)
                {
                    auto coreLogger = PELog::GetCoreLogger();
                    auto& sinks_vec = coreLogger->sinks();
                    sinks_vec.erase(std::remove(sinks_vec.begin(), sinks_vec.end(), log_sink_), sinks_vec.end());

                    size_t sink_capacity = 1000;
                    log_sink_ = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(sink_capacity);

                    coreLogger->sinks().push_back(log_sink_);
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Sample log message"))
            {
                PENGINE_CORE_WARN("Sample log message");
            }

            ImGui::Separator();
            float inputHeight = ImGui::GetFrameHeightWithSpacing();
            ImVec2 contentSize = ImGui::GetContentRegionAvail();

            ImGui::BeginChild("LogScrollingRegion", ImVec2(0, contentSize.y - inputHeight), false, window_flags_);
            RenderConsoleOutputWindow();
            ImGui::EndChild();

            RenderConsoleInputWindow();
        }
        ImGui::End();
    }

    void Console::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void Console::Open()
    {
        opened_ = true;
    }

    void Console::Close()
    {
        opened_ = false;
    }

    void Console::ToggleWindow()
    {
        if (opened_)
        {
            Close();
        }
        else
        {
            Open();
        }
    }

    void Console::RenderConsoleOutputWindow()
    {
        ImGui::BeginChild("LogScrollingRegion", ImVec2(0, 0), false, window_flags_);

        if (log_sink_)
        {
            auto messages = log_sink_->last_formatted();
            for (const auto& msg : messages)
            {
                ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                if (msg.find("[error]") != std::string::npos || msg.find("[critical]") != std::string::npos ||
                    msg.find("[ERROR]") != std::string::npos || msg.find("[CRITICAL]") != std::string::npos)
                {
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                }
                else if (msg.find("[warning]") != std::string::npos || msg.find("[WARN]") != std::string::npos)
                {
                    color = ImVec4(0.956f, 0.706f, 0.243f, 1.0f);
                }
                else if (msg.find("[debug]") != std::string::npos || msg.find("[trace]") != std::string::npos ||
                    msg.find("[DEBUG]") != std::string::npos || msg.find("[TRACE]") != std::string::npos)
                {
                    color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Cinza um pouco mais claro
                }
                else if (msg.find("[info]") != std::string::npos || msg.find("[INFO]") != std::string::npos)
                {
                    color = ImVec4(0.7f, 0.9f, 1.0f, 1.0f); // Azul claro para info
                }

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGuiIO& io = ImGui::GetIO();
                ImGui::PushFont(io.Fonts->Fonts[1]);
                ImGui::TextUnformatted(msg.c_str());
                ImGui::PopFont();

                ImGui::PopStyleColor();
            }
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f)
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }

    void Console::RenderConsoleInputWindow()
    {
        static char inputBuf[256] = "";

        ImGui::PushItemWidth(-1);
        if (ImGui::InputText("##ConsoleInput", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (strlen(inputBuf) > 0)
            {
                PENGINE_CORE_COMMAND("{}", std::string("> ") + inputBuf);
                CommandSystem::Get().Execute(inputBuf);
                inputBuf[0] = '\0';
            }
        }
        ImGui::PopItemWidth();
    }
} // ProEngine
