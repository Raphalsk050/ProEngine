cmake_minimum_required(VERSION 3.13)
project(filament LANGUAGES CXX)

set(FILAMENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (APPLE)
    file(GLOB FILAMENT_PLATFORM_LIBS
            "${CMAKE_CURRENT_SOURCE_DIR}/macos/*.a")
    # Common macOS frameworks required by Filament
    set(FILAMENT_PLATFORM_DEPS
            "-framework Metal" "-framework CoreVideo" "-framework Cocoa")
elseif (UNIX)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        file(GLOB FILAMENT_PLATFORM_LIBS
                "${CMAKE_CURRENT_SOURCE_DIR}/linux/arm64-v8a/*.a")
    else()
        file(GLOB FILAMENT_PLATFORM_LIBS
                "${CMAKE_CURRENT_SOURCE_DIR}/linux/x86_64/*.a")
    endif()
    file(GLOB FILAMENT_EXTRA_LIBS
            "${CMAKE_CURRENT_SOURCE_DIR}/linux/lib*.a")
    list(APPEND FILAMENT_PLATFORM_LIBS ${FILAMENT_EXTRA_LIBS})
    set(FILAMENT_PLATFORM_DEPS dl pthread c++)
endif()

add_library(filament INTERFACE)
target_include_directories(filament INTERFACE "${FILAMENT_INCLUDE_DIR}")
target_link_libraries(filament INTERFACE ${FILAMENT_PLATFORM_LIBS} ${FILAMENT_PLATFORM_DEPS})

