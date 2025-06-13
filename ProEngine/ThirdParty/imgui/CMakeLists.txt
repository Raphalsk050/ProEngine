cmake_minimum_required(VERSION 3.16)
project(ImGui)

if(POLICY CMP0065)
    cmake_policy(SET CMP0065 NEW)
endif()

if(POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)
endif()

if(POLICY CMP0179)
    cmake_policy(SET CMP0179 NEW)
endif()


# List of sources and headers
set(IMGUI_SOURCES
        imconfig.h
        imgui.h
        imgui.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        imgui_demo.cpp
        examples/imgui_impl_opengl3.h
        examples/imgui_impl_opengl3.cpp
        examples/imgui_impl_glfw.h
        examples/imgui_impl_glfw.cpp
)

# Create static library
add_library(ImGui STATIC ${IMGUI_SOURCES})

# Enforce C++17
target_compile_features(ImGui PUBLIC cxx_std_17)

# Configure output directories per configuration
foreach(cfg IN ITEMS Debug Release)
    set_target_properties(ImGui PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin/${cfg}/ImGui
            LIBRARY_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin/${cfg}/ImGui
            RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin/${cfg}/ImGui
    )
endforeach()

# Platform-specific settings
if(WIN32)
    # Use static runtime on MSVC
    set_property(TARGET ImGui PROPERTY MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(UNIX)
    # Enable Position Independent Code for Linux
    set_property(TARGET ImGui PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# Configuration-specific compile options
target_compile_options(ImGui PRIVATE
        # Debug: symbols on
        $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:MSVC>:/Zi>
        $<$<CXX_COMPILER_ID:GNU,Clang>:-g>
        >
        # Release: optimizations on
        $<$<CONFIG:Release>:
        $<$<CXX_COMPILER_ID:MSVC>:/O2>
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O3>
        >
)
