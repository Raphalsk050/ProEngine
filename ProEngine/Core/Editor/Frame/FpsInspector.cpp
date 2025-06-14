#include "FpsInspector.h"

#include "Core/Time.h"
#include "Core/Application/Application.h"

namespace ProEngine
{
    FpsInspector::FpsInspector(): opened_(false)
    {
        frames_ = std::vector<float>(max_frame_values_amount_window_);
        debug_name_ = "FpsInspector";
    }

    FpsInspector::~FpsInspector() = default;

    void FpsInspector::OnAttach()
    {
        Layer::OnAttach();
    }

    void FpsInspector::OnDetach()
    {
        Layer::OnDetach();
    }

    void FpsInspector::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);

        /*// only updates after update_tick_ms_
        int app_time_ms = Time::GetTime() * 1000.0f;
        if (app_time_ms % update_tick_ms_ != 0) return;*/

        float frame = round(1.0f / ts.GetSeconds());

        if (passed_frames_ >= amount_of_frames_to_get_avg_)
        {
            UpdateFPSHistory(GetAvgFPS());
            passed_frames_ = 0;
            return;
        }

        temp_frames_[passed_frames_] = frame;
        passed_frames_++;
        last_frame_amount_ = frame;
    }

    void FpsInspector::OnImGuiRender()
    {
        Layer::OnImGuiRender();
        if (!opened_) return;
        PlotGraph();
    }

    void FpsInspector::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void FpsInspector::Open()
    {
        opened_ = true;
    }

    void FpsInspector::Close()
    {
        opened_ = false;
    }

    void FpsInspector::ToggleWindow()
    {
        if (opened_)
            Close();
        else
            Open();
    }

    void FpsInspector::UpdateFPSHistory(const float frame)
    {
        if (frames_.size() > max_frame_values_amount_window_)
            frames_.erase(frames_.begin());
        frames_.push_back(frame);
    }

    void FpsInspector::PlotGraph()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
        ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 300.0f), ImVec2(5000.0f, 1000.0f));
        ImGui::Begin("FPSHistory", nullptr, window_flags);
        auto plot_width = ImGui::GetWindowSize().x - 50.0f;
        auto plot_height = 150.0f;
        auto text_height = ImGui::GetTextLineHeight();
        auto window_height = (ImGui::GetWindowSize().y - (plot_height + 2.0f * text_height)) * 0.5f;
        plot_size_ = ImVec2(plot_width, plot_height);

        ImGui::PopStyleVar(3);

        if (!frames_.empty())
        {
            std::vector<float> fps_smooth = SmoothValues(frames_, 1.0f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosX() + window_height);
            ImGui::BeginGroup();
            int last_fps_amount = static_cast<int>(fps_smooth.back());

            const char* last_fps_text = std::to_string(last_fps_amount).c_str();
            int offset = max_frame_values_amount_window_ / 1000 - 1;

            ImGui::PlotLines("##", fps_smooth.data(), 1000, offset, nullptr, 0.0f, 2000.0f, plot_size_);
            ImGui::SetNextItemWidth(plot_size_.x);
            ImGui::SliderInt("##", &max_frame_values_amount_window_, 30, 1000);

            ImGui::BeginGroup();
            ImGui::Text("0");
            ImGui::SameLine(plot_size_.x - ImGui::CalcTextSize(last_fps_text).x);
            ImGui::Text(last_fps_text);

            ImGui::EndGroup();
            ImGui::EndGroup();
        }

        ImGui::End();
    }

    float FpsInspector::GetAvgFPS()
    {
        float avg_fps = temp_frames_[0];
        for (int i = 1; i < std::size(temp_frames_); i++)
        {
            avg_fps += temp_frames_[i];
        }
        avg_fps /= std::size(temp_frames_);
        return avg_fps;
    }

    std::vector<float> FpsInspector::SmoothValues(const std::vector<float>& values, float alpha)
    {
        std::vector<float> smooth_values(values.size());

        if (values.empty())
            return smooth_values;

        smooth_values[0] = values[0];

        for (size_t i = 1; i < values.size(); ++i)
        {
            smooth_values[i] = smooth_values[i - 1] + alpha * (values[i] - smooth_values[i - 1]);
        }

        return smooth_values;
    }
}
