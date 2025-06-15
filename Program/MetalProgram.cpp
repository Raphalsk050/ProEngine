#include <ProEngine.h>
#include "MetalSampleLayer.h"

namespace ProEngine {
class MetalSample : public Application {
public:
    explicit MetalSample(const ApplicationSpecification& spec)
        : Application(spec) {
        PushLayer(new MetalSampleLayer());
    }
};

Application* CreateApplication(ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.Name = "MetalSample";
    spec.CommandLineArgs = args;
    spec.WindowWidth = 1600;
    spec.WindowHeight = 900;
    return new MetalSample(spec);
}
}
