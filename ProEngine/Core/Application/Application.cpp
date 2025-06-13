#include "Application.h"

namespace ProEngine {
  Application *Application::instance_ = nullptr;

  Application::Application(const ApplicationSpecification &specification) : specification_(specification) {
    PENGINE_CORE_ASSERT(!instance_, "Application already exists!");
    instance_ = this;
  }
}
