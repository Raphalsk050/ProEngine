# CMakeList.txt : CMake project for ProEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.2)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ProEngine")

add_library (ProEngine STATIC
	"ProEngine.h"
	"ProEngine.cpp"
)


# TODO: Add tests and install targets if needed.
