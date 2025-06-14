#include <ProEngine.h>

#include "SampleLayer.h"

using namespace std;

namespace ProEngine {
  class Sample : public Application {
  public:
    explicit Sample(const ApplicationSpecification &spec)
      : Application(spec) {
      PushLayer(new SampleLayer());
    }
  };

  Application *CreateApplication(ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.Name = "SampleApp";
    spec.CommandLineArgs = args;
    spec.WindowWidth = 1600;
    spec.WindowHeight = 900;

    return new Sample(spec);
  }
}
