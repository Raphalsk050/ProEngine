#pragma once
#include "Core/Layer/Layer.h"

namespace ProEngine {
class MetalSampleLayer : public Layer {
public:
    MetalSampleLayer();
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
};
}
