cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)
endif()

if(POLICY CMP0179)
    cmake_policy(SET CMP0179 NEW)
endif()

project(ProEngine LANGUAGES CXX)

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                     GLOBAL CONFIGURATION                ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                  THIRD-PARTY CONFIGURATION              ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
# Disable shared libs, installs, and tests
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_SKIP_INSTALL_RULES ON CACHE BOOL "" FORCE)

# GLFW-specific build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                 DEPENDENCY DIRECTORY SETUP              ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
set(spdlog_DIR ThirdParty/spdlog)
set(imgui_DIR ThirdParty/imgui)
set(glfw_DIR ThirdParty/glfw)
set(glad_DIR ThirdParty/glad)
set(glm_DIR ThirdParty/glm)


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                 IMPORT EXTERNAL PROJECTS                ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
add_subdirectory(${spdlog_DIR})
add_subdirectory(${imgui_DIR})
add_subdirectory(${glfw_DIR})
add_subdirectory(${glad_DIR})
add_subdirectory(${glm_DIR})

target_compile_definitions(ImGui PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                   PLATFORM DETECTION                    ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
if (WIN32)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_WINDOWS)
elseif (APPLE)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_APPLE)
elseif (UNIX)
    set(PROENGINE_PLATFORM_DEFINE PROENGINE_PLATFORM_LINUX)
endif ()


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃           ENGINE LIBRARY CREATOR HELPER FUNCTION        ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
function(engine_add_library target_name)
    add_library(${target_name} STATIC ${ARGN})

    target_compile_definitions(${target_name} PUBLIC
            ${PROENGINE_PLATFORM_DEFINE}
            $<$<CONFIG:Debug>:DEBUG>
            $<$<CONFIG:Release>:NDEBUG>
    )

    target_include_directories(${target_name} PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_features(${target_name} PUBLIC cxx_std_20)

    # Enable Unity Build for faster compilation
    set_target_properties(${target_name} PROPERTIES
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 16
    )
endfunction()


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                  SYSTEM DEPENDENCIES                    ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
find_package(OpenGL REQUIRED)

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                     ENGINE MODULES                      ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

# ── Rendering Library ─────────────────────────────────────
engine_add_library(PERendering
        Core/Renderer/GraphicsContext.h
        Core/Renderer/GraphicsContext.cpp
        Core/Renderer/Renderer.h
        Core/Renderer/Renderer.cpp
        Core/Renderer/RenderCommand.h
        Core/Renderer/RenderCommand.cpp
        Core/Renderer/RendererAPI.h
        Core/Renderer/RendererAPI.cpp
)
target_link_libraries(PERendering
        spdlog
        glfw
        glad
)
target_include_directories(PERendering PUBLIC
        PECore
        ${spdlog_DIR}/include
        ${glfw_DIR}/include
        ${glad_DIR}/include
        ${glm_DIR}
)

# ── Core Library ──────────────────────────────────────────
engine_add_library(PECore
        ProEngine.h
        Core/Log/PELog.h
        Core/Log/PELog.cpp
        Core/Assert/Assert.h
        Core/Debug/Instrumentor.h
        Core/Time.h
        Core/Timestep.h
)
target_link_libraries(PECore PUBLIC
        spdlog
        glfw
        glad
)
target_include_directories(PECore PUBLIC
        ${spdlog_DIR}/include
        ${glfw_DIR}/include
        ${glad_DIR}/include
)

# ── Layer ─────────────────────────────────────────────────
engine_add_library(PELayer
        Core/Layer/Layer.h
        Core/Layer/Layer.cpp
        Core/Layer/LayerStack.h
        Core/Layer/LayerStack.cpp
)
target_link_libraries(PELayer PUBLIC
        ImGui
        PECore
        PEEvent
)

# ── Input Handling ────────────────────────────────────────
engine_add_library(PEInput
        Core/Input/KeyCodes.h
        Core/Input/MouseCodes.h
        Core/Input/Input.h
        Core/Input/Input.cpp
)
target_link_libraries(PEInput PUBLIC
        PECore
        glfw
)
target_include_directories(PEInput PUBLIC
        ${glm_DIR}
        ${imgui_DIR}
)

# ── Event System ─────────────────────────────────────────
engine_add_library(PEEvent
        Core/Event/Event.h
        Core/Event/Event.cpp
        Core/Event/KeyEvent.h
        Core/Event/MouseEvent.h
        Core/Event/WindowApplicationEvent.h
)
target_link_libraries(PEEvent PUBLIC
        PECore
)
target_include_directories(PEEvent PUBLIC
        ${glm_DIR}
)

# ── Application Framework ──────────────────────────────────
engine_add_library(PEWindow
    Core/Window/GenericGLFWWindow.h
    Core/Window/GenericGLFWWindow.cpp
    Core/Window/Window.h
    Core/Window/Window.cpp
)
target_link_libraries(PEWindow
        PERendering
        spdlog
        glfw
        glad
        glm
)
target_include_directories(PEWindow PUBLIC
        ${glm_DIR}
)

# ── Application Framework ──────────────────────────────────
engine_add_library(PEApplication
        Core/Application/EntryPoint.h
        Core/Application/Application.h
        Core/Application/Application.cpp
        Core/Imgui/ImguiLayer.h
        Core/Imgui/ImguiLayer.cpp
)
target_link_libraries(PEApplication PUBLIC
        PECore
        PEEvent
        PELayer
        PEWindow
)
target_include_directories(PEApplication PUBLIC
        ${imgui_DIR}
        ${glm_DIR}
)

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                     PLATFORM MODULES                    ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
engine_add_library(PEPlatform
        Platform/OpenGL/OpenGLContext.cpp
        Platform/OpenGL/OpenGLContext.h
        Platform/OpenGL/OpenGLRendererAPI.h
        Platform/OpenGL/OpenGLRendererAPI.cpp
)
target_link_libraries(PEPlatform
        spdlog
        glfw
        glad
)
target_include_directories(PEPlatform PUBLIC
        ${glm_DIR}
)

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                ENGINE INTERFACE AGGREGATE               ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
add_library(ProEngine INTERFACE)

target_link_libraries(ProEngine INTERFACE
        PECore
        PEWindow
        PEApplication
        PEPlatform
        PEEvent
        PEInput
)


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃            IMGUI CONSUMER CONFIGURATION                 ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
target_include_directories(ImGui PUBLIC
        ${glad_DIR}/include
        ${glfw_DIR}/include
)
target_link_libraries(ImGui PUBLIC
        glfw
        glad
        ${OPENGL_LIBRARIES}
        ${CMAKE_DL_LIBS}
)
