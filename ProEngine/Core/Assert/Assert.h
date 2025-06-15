#pragma once
#include "Config.h"
#include "Core/Log/PELog.h"
#include <filesystem>

namespace ProEngine {
#pragma once

#ifdef PROENGINE_ENABLE_ASSERTS
  // Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
  // provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define PENGINE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { PENGINE##type##ERROR(msg, __VA_ARGS__); PENGINE_DEBUGBREAK(); } }
#define PENGINE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PENGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define PENGINE_INTERNAL_ASSERT_NO_MSG(type, check) PENGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PENGINE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define PENGINE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define PENGINE_INTERNAL_ASSERT_GET_MACRO(...) PENGINE_EXPAND_MACRO( PENGINE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PENGINE_INTERNAL_ASSERT_WITH_MSG, PENGINE_INTERNAL_ASSERT_NO_MSG) )

  // Currently accepts at least the condition and one additional parameter (the message) being optional
#define PENGINE_ASSERT(...) PENGINE_EXPAND_MACRO( PENGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define PENGINE_CORE_ASSERT(...) PENGINE_EXPAND_MACRO( PENGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define PENGINE_ASSERT(...)
#define PENGINE_CORE_ASSERT(...)
#endif
}
