#pragma once
#include "Core/TimeStep.h"
#include "Core/Event/Event.h"
#include "Core/Layer/Layer.h"

namespace ProEngine {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        void OnUpdate(Timestep ts) override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { block_events_ = block; }

        void SetDarkThemeColors();

        void ShowMetricsWindow();

        uint32_t GetActiveWidgetID() const;

    private:
        // const char* font_path_ = "Assets/Fonts/Inter/Inter-Regular.ttf";
        //const char* font_path_ = "../ForgeEngine/Assets/Fonts/Inter/Inter-Regular.ttf";
        // const char* font_path_ = "../ForgeEngine/Assets/Fonts/JetBrains/JetBrainsMonoNerdFont-Regular.ttf";
        bool block_events_ = true;
    };
}  // namespace ProEngine