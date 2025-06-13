#pragma once
#include <string>
#include "PEPCH.h"

using namespace std;

namespace ProEngine {
  struct ApplicationCommandLineArgs {
    int Count = 0;
    char **Args = nullptr;

    const char *operator[](int index) const {
      PENGINE_CORE_ASSERT(index < Count);
      return Args[index];
    }
  };

  struct ApplicationSpecification {
    string Name = "Pro-Engine Application";
    string WorkingDirectory;
    uint32_t WindowWidth = 1280;
    uint32_t WindowHeight = 720;
    ApplicationCommandLineArgs CommandLineArgs;
  };

  class Application {
  public:
    Application(const ApplicationSpecification &specification);

    ~Application();

  private:
    ApplicationSpecification specification_;

  private:
    static Application *instance_;
  };

  Application *CreateApplication(ApplicationCommandLineArgs args);
}
