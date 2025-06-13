#pragma once
#include "Core/Layer/Layer.h"

namespace ProEngine
{
    class SampleLayer : public Layer
    {
    public:
        SampleLayer();
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
    };
} // ProEngine
