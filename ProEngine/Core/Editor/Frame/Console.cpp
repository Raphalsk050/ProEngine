#include "Console.h"


#include "imgui.h"

namespace ProEngine
{
    Console::Console(): open_(false)
    {
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
            PENGINE_CORE_INFO("MainUI Layer Attached. Sink de UI adicionado ao CoreLogger.");
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
        if (ImGui::Begin("Console", nullptr))
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
                PENGINE_CORE_INFO("Sample log message");
            }

            ImGui::Separator();
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
                        color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
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
                    ImGui::TextUnformatted(msg.c_str());
                    ImGui::PopStyleColor();
                }
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f)
            {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }
        ImGui::End();
    }

    void Console::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void Console::OpenConsole()
    {
        open_ = true;
    }

    void Console::CloseConsole()
    {
        open_ = false;
    }
} // ProEngine
