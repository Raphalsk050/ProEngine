#include <ProEngine.h>

using namespace std;

namespace ProEngine {
  class Sample : public Application {
  public:
    explicit Sample(const ApplicationSpecification &spec)
      : Application(spec) {
      //PushLayer(new NidavellirLayer());
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
