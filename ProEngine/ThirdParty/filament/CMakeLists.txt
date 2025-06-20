cmake_minimum_required(VERSION 3.13)
project(filament LANGUAGES CXX)

# Filament's prebuilt libraries included in this repository target Android and
# are incompatible with standard Linux or macOS toolchains.  To keep the build
# working we expose Filament headers only.

set(FILAMENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(filament INTERFACE)
target_include_directories(filament INTERFACE "${FILAMENT_INCLUDE_DIR}")

add_library(filament_stub STATIC
    stub/EngineStub.cpp
    stub/MinimalScene.cpp
)
target_include_directories(filament_stub PUBLIC "${FILAMENT_INCLUDE_DIR}")
target_link_libraries(filament INTERFACE filament_stub)

