#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace ProEngine {
  class PELog {
  public:
    // Initialize the Logger (call once at startup)
    static void Init(const std::string &engineName = "PENGINE");

    // Retrieve the core logger
    inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }

    // (Optional) Retrieve a client/user logger
    inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };
}
//#define DEBUG
#ifdef DEBUG

#define PENGINE_CORE_TRACE(...)    ::ProEngine::PELog::GetCoreLogger()->trace(__VA_ARGS__)
#define PENGINE_CORE_INFO(...)     ::ProEngine::PELog::GetCoreLogger()->info(__VA_ARGS__)
#define PENGINE_CORE_WARN(...)     ::ProEngine::PELog::GetCoreLogger()->warn(__VA_ARGS__)
#define PENGINE_CORE_ERROR(...)    ::ProEngine::PELog::GetCoreLogger()->error(__VA_ARGS__)
#define PENGINE_CORE_COMMAND(...)  ::ProEngine::PELog::GetCoreLogger()->debug(__VA_ARGS__)
#define PENGINE_CORE_CRITICAL(...) ::ProEngine::PELog::GetCoreLogger()->critical(__VA_ARGS__)

#else

#define PENGINE_CORE_TRACE(...)    do {} while(false)
#define PENGINE_CORE_INFO(...)     do {} while(false)
#define PENGINE_CORE_WARN(...)     do {} while(false)
#define PENGINE_CORE_ERROR(...)    do {} while(false)
#define PENGINE_CORE_CRITICAL(...) do {} while(false)

#endif
