#pragma once
#include "ProEngine.h"
#include "Core/Application/Application.h"

extern ProEngine::Application *ProEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
  ProEngine::PELog::Init();

  auto app = ProEngine::CreateApplication({argc, argv});
  PENGINE_CORE_INFO("Application created");

  app->Run();

  return 0;
}
