cmake_minimum_required(VERSION 3.13)
project(filament LANGUAGES CXX)

# Only the headers are usable on non-Android platforms since the provided
# libraries were built with the Android NDK and are incompatible with Linux or
# macOS toolchains.  We therefore expose Filament as a header-only interface
# library.  On Apple platforms we additionally try to import the prebuilt
# archives if available.

set(FILAMENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (APPLE)
    # Prebuilt macOS archives may not be present.  Attempt to gather them and
    # link if found, otherwise fall back to header-only usage.
    file(GLOB FILAMENT_MAC_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/macos/*.a")
    if (FILAMENT_MAC_LIBS)
        add_library(filament STATIC IMPORTED GLOBAL)
        set_target_properties(filament PROPERTIES
            IMPORTED_LOCATION "${FILAMENT_MAC_LIBS}"
            INTERFACE_INCLUDE_DIRECTORIES "${FILAMENT_INCLUDE_DIR}"
            LINKER_LANGUAGE CXX
        )
        target_link_options(filament INTERFACE
            "-framework Metal" "-framework CoreVideo" "-framework Cocoa")
    else()
        add_library(filament INTERFACE)
        target_include_directories(filament INTERFACE "${FILAMENT_INCLUDE_DIR}")
    endif()
else()
    # Non-Apple platforms only use the headers.
    add_library(filament INTERFACE)
    target_include_directories(filament INTERFACE "${FILAMENT_INCLUDE_DIR}")
endif()

