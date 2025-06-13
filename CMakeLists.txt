# CMakeList.txt : CMake project for ProEngine, include source and define
# project specific logic here.
#

# Silence CMP0126 warning from CMakeFindBinUtils on newer CMake versions
if(POLICY CMP0126)
  cmake_policy(SET CMP0126 NEW)
endif()

# Ensure add_library uses deduplicated link lines with modern linker behavior
if(POLICY CMP0156)
  cmake_policy(SET CMP0156 NEW)
endif()

# Ensure static libraries keep the first occurrence when deduplicating
if(POLICY CMP0179)
  cmake_policy(SET CMP0179 NEW)
endif()

project(ProEngineApplication)
cmake_minimum_required (VERSION 3.2)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory (ProEngine)
add_subdirectory (Program)

