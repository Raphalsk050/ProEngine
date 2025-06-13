#include "SampleLayer.h"

namespace ProEngine {
    SampleLayer::SampleLayer() : Layer("SampleLayer")
    {
    }

    void SampleLayer::OnAttach()
    {
        Layer::OnAttach();
    }

    void SampleLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void SampleLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void SampleLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
    }

    void SampleLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }
} // ProEngine