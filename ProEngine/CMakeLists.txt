cmake_minimum_required(VERSION 3.16)
project(ProEngine LANGUAGES CXX)

# ===========================================
# GLOBAL SETTINGS
# ===========================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build optimizations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ===========================================
# OPENGL SYSTEM LIBRARIES
# ===========================================
find_package(OpenGL REQUIRED)

# Enable compilation cache if available
if(NOT DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
        message(STATUS "Using ccache for faster compilation")
    endif()
endif()

# ===========================================
# PLATFORM-SPECIFIC DEFINITIONS
# ===========================================
if(WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(NOMINMAX)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()

# ===========================================
# COMPILER OPTIMIZATIONS
# ===========================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Try to use faster linkers
    if(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "-fuse-ld")
        find_program(MOLD_FOUND mold)
        find_program(LLD_FOUND ld.lld)
        find_program(GOLD_FOUND ld.gold)

        if(MOLD_FOUND)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=mold")
            message(STATUS "Using mold linker for faster linking")
        elseif(LLD_FOUND)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
            message(STATUS "Using lld linker for faster linking")
        elseif(GOLD_FOUND)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
            message(STATUS "Using gold linker for faster linking")
        endif()
    endif()

    # Compiler optimizations
    add_compile_options(
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-march=native>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
    )
endif()

# MSVC specific settings
if(MSVC)
    # Enable hot reload for MSVC
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
    endif()
    
    # Compiler flags
    add_compile_options(
        /W4
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
    )
endif()

# ===========================================
# THIRD PARTY CMAKE VARIABLES
# ===========================================
# Disable all installations and tests for third party libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_SKIP_INSTALL_RULES ON CACHE BOOL "" FORCE)

# GLFW settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# glslang settings
set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
set(ENABLE_CTEST OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL ON CACHE BOOL "" FORCE)
set(BUILD_EXTERNAL OFF CACHE BOOL "" FORCE)

# Assimp settings
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

# ===========================================
# THIRD PARTY DIRECTORIES
# ===========================================
set(PROENGINE_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
set(glfw_DIR ${PROENGINE_THIRD_PARTY_DIR}/glfw)
set(glad_DIR ${PROENGINE_THIRD_PARTY_DIR}/glad)
set(glm_DIR ${PROENGINE_THIRD_PARTY_DIR}/glm)
set(entt_DIR ${PROENGINE_THIRD_PARTY_DIR}/entt)
set(spdlog_DIR ${PROENGINE_THIRD_PARTY_DIR}/spdlog)
set(imgui_DIR ${PROENGINE_THIRD_PARTY_DIR}/imgui)
set(ImGuizmo_DIR ${PROENGINE_THIRD_PARTY_DIR}/ImGuizmo)

# ===========================================
# ADD THIRD PARTY LIBRARIES
# ===========================================
# Check if directories exist before adding
if(EXISTS ${glad_DIR}/CMakeLists.txt)
    add_subdirectory(${glad_DIR})
else()
    message(WARNING "glad not found at ${glad_DIR}")
endif()

if(EXISTS ${glfw_DIR}/CMakeLists.txt)
    add_subdirectory(${glfw_DIR})
else()
    message(WARNING "GLFW not found at ${glfw_DIR}")
endif()

if(EXISTS ${glm_DIR}/CMakeLists.txt)
    add_subdirectory(${glm_DIR})
else()
    message(WARNING "GLM not found at ${glm_DIR}")
endif()

if(EXISTS ${spdlog_DIR}/CMakeLists.txt)
    add_subdirectory(${spdlog_DIR})
else()
    message(WARNING "spdlog not found at ${spdlog_DIR}")
endif()

if(EXISTS ${entt_DIR}/CMakeLists.txt)
    add_subdirectory(${entt_DIR})
else()
    message(WARNING "EnTT not found at ${entt_DIR}")
endif()

if(EXISTS ${imgui_DIR}/CMakeLists.txt)
    add_subdirectory(${imgui_DIR})
else()
    message(WARNING "ImGui not found at ${imgui_DIR}")
endif()

if(EXISTS ${ImGuizmo_DIR}/CMakeLists.txt)
    add_subdirectory(${ImGuizmo_DIR})
else()
    message(WARNING "ImGuizmo not found at ${ImGuizmo_DIR}")
endif()

# ===========================================
# ASSIMP CONFIGURATION
# ===========================================
include(FetchContent)

# Option to use precompiled or fetch Assimp
option(USE_PRECOMPILED_ASSIMP "Use precompiled Assimp instead of FetchContent" OFF)

if(USE_PRECOMPILED_ASSIMP)
    # Precompiled Assimp
    add_library(assimp SHARED IMPORTED GLOBAL)
    
    set(ASSIMP_ROOT_DIR "${PROENGINE_THIRD_PARTY_DIR}/assimp")
    set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT_DIR}/include")
    
    if(WIN32)
        set(ASSIMP_DLL "${ASSIMP_ROOT_DIR}/bin/assimp-vc143-mt.dll")
        set(ASSIMP_LIB "${ASSIMP_ROOT_DIR}/lib/assimp-vc143-mt.lib")
        
        if(NOT EXISTS "${ASSIMP_LIB}")
            message(FATAL_ERROR "Assimp .lib file not found at: ${ASSIMP_LIB}")
        endif()
        
        set_target_properties(assimp PROPERTIES
            IMPORTED_LOCATION "${ASSIMP_DLL}"
            IMPORTED_IMPLIB "${ASSIMP_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIR}"
        )
    else()
        message(FATAL_ERROR "Precompiled Assimp only configured for Windows")
    endif()
else()
    # Fetch Assimp from source
    message(STATUS "Fetching Assimp from source...")
    
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1
    )
    
    FetchContent_MakeAvailable(assimp)
endif()

# ===========================================
# PLATFORM DETECTION
# ===========================================
if(WIN32)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_WINDOWS)
elseif(APPLE)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_APPLE)
elseif(UNIX)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_LINUX)
endif()

# ===========================================
# PROENGINE LIBRARY
# ===========================================
add_library(ProEngine STATIC
    ProEngine.h
    ProEngine.cpp
    PEPCH.h
)

# Set properties
set_target_properties(ProEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

# Platform and configuration definitions
target_compile_definitions(ProEngine
    PUBLIC
        ${PROENGINE_PLATFORM_DEFINE}
        $<$<CONFIG:Debug>:PROENGINE_DEBUG>
        $<$<CONFIG:Release>:PROENGINE_RELEASE>
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# Include directories
target_include_directories(ProEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${glm_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(ProEngine
    PUBLIC
        assimp
        $<TARGET_NAME_IF_EXISTS:glfw>
        $<TARGET_NAME_IF_EXISTS:glad>
        $<TARGET_NAME_IF_EXISTS:glm-header-only>
        $<TARGET_NAME_IF_EXISTS:spdlog>
        $<TARGET_NAME_IF_EXISTS:EnTT>
        $<TARGET_NAME_IF_EXISTS:ImGui>
        $<TARGET_NAME_IF_EXISTS:ImGuizmo>
)

# Additional include directories for third-party libraries
if(TARGET spdlog)
    target_include_directories(ProEngine PUBLIC ${spdlog_DIR}/include)
endif()
if(TARGET ImGui)
    target_include_directories(ProEngine PUBLIC ${imgui_DIR})
    target_compile_definitions(ImGui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_include_directories(ImGui PUBLIC
        ${glad_DIR}/include
        ${glfw_DIR}/include
    )
endif()
if(TARGET ImGuizmo)
    target_include_directories(ProEngine PUBLIC ${ImGuizmo_DIR})
    target_include_directories(ImGuizmo PUBLIC
        ${imgui_DIR}
    )
endif()

# MSVC specific debug settings
if(MSVC)
    set_property(TARGET ProEngine PROPERTY 
        MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug>,EditAndContinue,ProgramDatabase>"
    )
endif()

# ===========================================
# STBI IMAGE (if needed)
# ===========================================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stbimage/stb_image.h")
    add_library(stb_image STATIC
        ThirdParty/stbimage/stb_image.h
        ThirdParty/stbimage/stb_image.cpp
        PEPCH.h
    )
     target_include_directories(stb_image PUBLIC "./")
    target_link_libraries(ProEngine PUBLIC stb_image)
    set_target_properties(stb_image PROPERTIES UNITY_BUILD OFF)
endif()

# ===========================================
# POST BUILD COMMANDS
# ===========================================
# Copy DLLs if using precompiled Assimp
if(USE_PRECOMPILED_ASSIMP AND WIN32)
    add_custom_command(TARGET ProEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSIMP_DLL}"
            $<TARGET_FILE_DIR:ProEngine>
        COMMENT "Copying Assimp DLL to output directory"
    )
endif()

# Copy assets if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
    add_custom_command(TARGET ProEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
            $<TARGET_FILE_DIR:ProEngine>/Assets
        COMMENT "Copying assets to output directory"
    )
endif()

# ===========================================
# BUILD CONFIGURATION OUTPUT
# ===========================================
message(STATUS "====================================")
message(STATUS "ProEngine Build Configuration:")
message(STATUS "  Platform:        ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Platform Define: ${PROENGINE_PLATFORM_DEFINE}")
message(STATUS "  C++ Standard:    ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:        ${CMAKE_CXX_COMPILER_ID}")
if(USE_PRECOMPILED_ASSIMP)
    message(STATUS "  Assimp:          Precompiled")
else()
    message(STATUS "  Assimp:          Built from source")
endif()
if(CMAKE_CXX_COMPILER_LAUNCHER)
    message(STATUS "  Using ccache:    YES")
else()
    message(STATUS "  Using ccache:    NO")
endif()
message(STATUS "")
message(STATUS "Third-party libraries found:")
if(TARGET glfw)
    message(STATUS "  - GLFW")
endif()
if(TARGET glad)
    message(STATUS "  - glad")
endif()
if(TARGET glm)
    message(STATUS "  - GLM")
endif()
if(TARGET spdlog)
    message(STATUS "  - spdlog")
endif()
if(TARGET EnTT)
    message(STATUS "  - EnTT")
endif()
if(TARGET ImGui)
    message(STATUS "  - ImGui")
endif()
if(TARGET ImGuizmo)
    message(STATUS "  - ImGuizmo")
endif()
message(STATUS "====================================")


# ===========================================
# EXAMPLE EXECUTABLE (optional)
# ===========================================
option(BUILD_EXAMPLES "Build example applications" OFF)

if(BUILD_EXAMPLES)
    add_executable(ProEngineExample
        examples/main.cpp
    )
    
    target_link_libraries(ProEngineExample
        PRIVATE
            ProEngine
    )
    
    # Copy DLLs to example output directory
    if(USE_PRECOMPILED_ASSIMP AND WIN32)
        add_custom_command(TARGET ProEngineExample POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${ASSIMP_DLL}"
                $<TARGET_FILE_DIR:ProEngineExample>
        )
    endif()
endif()