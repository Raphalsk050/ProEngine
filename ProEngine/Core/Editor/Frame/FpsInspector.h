#pragma once
#include <imgui.h>
#include "Core/Layer/Layer.h"

namespace ProEngine
{
    class FpsInspector : public Layer
    {
    public:
        FpsInspector();
        ~FpsInspector() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
        void Open();
        void Close();
        void ToggleWindow();

    private:
        static std::vector<float> SmoothValues(const std::vector<float>& values, float alpha);
        void UpdateFPSHistory(float frame);
        void PlotGraph();
        float GetAvgFPS();

        std::vector<float> frames_;
        constexpr static int amount_of_frames_to_get_avg_ = 10;
        float temp_frames_[amount_of_frames_to_get_avg_] = {};
        int max_frame_values_amount_window_ = 1000;
        bool opened_ = false;
        float last_frame_amount_ = 0.0f;
        int max_values_count_ = 0;
        int update_tick_ms_ = 10;
        int offset_ = 0;
        int passed_frames_ = 0;
        ImVec2 plot_size_ = ImVec2(300, 100);
        ImVec2 window_size_ = ImVec2(400, 200);
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    };
}
