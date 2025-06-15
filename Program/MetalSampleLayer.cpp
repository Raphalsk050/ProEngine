#include "MetalSampleLayer.h"
#include "PEPCH.h"

namespace ProEngine {
MetalSampleLayer::MetalSampleLayer() : Layer("MetalSampleLayer") {}

void MetalSampleLayer::OnAttach() {
    Layer::OnAttach();
}

void MetalSampleLayer::OnDetach() {
    Layer::OnDetach();
}

void MetalSampleLayer::OnUpdate(Timestep ts) {
    Layer::OnUpdate(ts);
}
}
