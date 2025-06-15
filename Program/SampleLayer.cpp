#include "SampleLayer.h"

#include "Core/Application/Application.h"
#include "Core/Scene/EntityHandle.h"

namespace ProEngine
{
    SampleLayer::SampleLayer() : Layer("SampleLayer")
    {
    }

    void SampleLayer::OnAttach()
    {
        Layer::OnAttach();
        Application::Get().GetActiveScene()->CreateEntity();

        for (int i = 0; i < 10; i++)
        {
            Application::Get().GetActiveScene()->CreateEntity("Entity(" + std::to_string(i) + ")");
        }
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
