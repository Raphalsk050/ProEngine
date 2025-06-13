#include "PELog.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ProEngine
{
    std::shared_ptr<spdlog::logger> PELog::s_CoreLogger;
    std::shared_ptr<spdlog::logger> PELog::s_ClientLogger;

    void PELog::Init(const std::string& engineName)
    {
        // optional: control compile-time log level
#if defined(DEBUG) || defined(_DEBUG)
        spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif

        // Pattern: [HH:MM:SS] [logger] [level] message
        spdlog::set_pattern("[%T] [%n] [%^%l%$] %v");

        // Create a color console sink
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::trace);

        // Create a file sink
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("engine.log", true);
        fileSink->set_level(spdlog::level::trace);

        // Core logger uses both console and file
        s_CoreLogger = std::make_shared<spdlog::logger>(engineName, spdlog::sinks_init_list{consoleSink, fileSink});
        s_CoreLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_CoreLogger);

        // (Optional) Client logger
        s_ClientLogger = std::make_shared<spdlog::logger>("APPLICATION", spdlog::sinks_init_list{consoleSink, fileSink});
        s_ClientLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_ClientLogger);
    }
} // namespace ForgeEngine
