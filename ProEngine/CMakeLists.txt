cmake_minimum_required(VERSION 3.16)

if (POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)
endif ()

if (POLICY CMP0179)
    cmake_policy(SET CMP0179 NEW)
endif ()

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
# ┃              ENGINE FEATURE FLAGS (OPTIONS)             ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

# Helper macro to define an option and propagate it as a compile definition
macro(proengine_define_option FLAG_NAME DESCRIPTION DEFAULT)
    option(${FLAG_NAME} "${DESCRIPTION}" ${DEFAULT})
    if (${FLAG_NAME})
        add_compile_definitions(${FLAG_NAME})
    endif ()
endmacro()

proengine_define_option(PROENGINE_ENABLE_EDITOR "Enable runtime editor module" OFF)
proengine_define_option(PROENGINE_DEBUG_LAYERS "Enable debug rendering for layers" OFF)
proengine_define_option(PROENGINE_ENABLE_ASSERTS "Enable engine asserts" OFF)


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
set(entt_DIR ThirdParty/entt)
set(stbimage_DIR ThirdParty/stbimage)


# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃                 IMPORT EXTERNAL PROJECTS                ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
add_subdirectory(${spdlog_DIR})
add_subdirectory(${imgui_DIR})
add_subdirectory(${glfw_DIR})
add_subdirectory(${glad_DIR})
add_subdirectory(${glm_DIR})
add_subdirectory(${entt_DIR})
add_subdirectory(${stbimage_DIR})

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
        Core/Renderer/Buffer.h
        Core/Renderer/Buffer.cpp
        Core/Renderer/Renderer.h
        Core/Renderer/Renderer.cpp
        Core/Renderer/Renderer3D.h
        Core/Renderer/Renderer3D.cpp
        Core/Renderer/RenderCommand.h
        Core/Renderer/RenderCommand.cpp
        Core/Renderer/Mesh.h
        Core/Renderer/Mesh.cpp
        Core/Renderer/Material.h
        Core/Renderer/RendererAPI.h
        Core/Renderer/RendererAPI.cpp
        Core/Renderer/Framebuffer.h
        Core/Renderer/Framebuffer.cpp
        Core/Renderer/Shader.h
        Core/Renderer/Shader.cpp
        Core/Renderer/UniformBuffer.h
        Core/Renderer/UniformBuffer.cpp
        Core/Renderer/Texture.h
        Core/Renderer/Texture.cpp
        Core/Renderer/VertexArray.h
        Core/Renderer/VertexArray.cpp
        Core/Renderer/InstancedRenderer.h
        Core/Renderer/InstancedRenderer.cpp
)
target_link_libraries(PERendering
        spdlog
        glfw
        glad
        Entt
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
        spdlog
        glfw
        glad
        glm
)
target_include_directories(PEWindow PUBLIC
        ${glm_DIR}
)

# ── Scene ──────────────────────────────────────────────────
engine_add_library(PEScene
        Core/Scene/Scene.h
        Core/Scene/Scene.cpp
        Core/Scene/EntityHandle.h
        Core/Scene/EntityHandle.cpp
        Core/Scene/Components.h
)
target_link_libraries(PEScene
        PERendering
        spdlog
        glfw
        glad
        glm
        Entt
)
target_include_directories(PEScene PUBLIC
        ${glm_DIR}
)

# ── Camera ──────────────────────────────────────────────────
engine_add_library(PECamera
        Core/Camera/Camera.h
        Core/Camera/Camera3D.h
        Core/Camera/Camera3D.cpp
        Core/Camera/Camera3DController.h
        Core/Camera/Camera3DController.cpp
)
target_link_libraries(PECamera
        PERendering
        spdlog
        glfw
        glad
        glm
        Entt
)
target_include_directories(PECamera PUBLIC
        ${glm_DIR}
)

# ── Editor ──────────────────────────────────────────────────
if (PROENGINE_ENABLE_EDITOR)
    engine_add_library(PEEditor
            Core/Editor/ImguiLayer.h
            Core/Editor/ImguiLayer.cpp
            Core/Editor/Frame/Console.h
            Core/Editor/Frame/Console.cpp
            Core/Editor/Frame/FpsInspector.h
            Core/Editor/Frame/FpsInspector.cpp
            Core/Editor/Frame/MainEditorInterface.h
            Core/Editor/Frame/MainEditorInterface.cpp
            Core/Editor/Frame/Hierarchy/HierarchyInspector.h
            Core/Editor/Frame/Hierarchy/HierarchyInspector.cpp
            Core/Editor/AnimatedPopup.h
            Core/Editor/SimpleAnimatedPopup.h
            Core/Editor/CommandSystem.h
    )
    target_link_libraries(PEEditor PUBLIC
            spdlog
            ImGui
            Entt
            glm
    )
    target_include_directories(PEEditor PUBLIC
            ${imgui_DIR}
            ${glm_DIR}
            ${entt_DIR}
    )
endif ()

# ── Application Framework ──────────────────────────────────
engine_add_library(PEApplication
        Core/Application/EntryPoint.h
        Core/Application/Application.h
        Core/Application/Application.cpp
)
target_link_libraries(PEApplication PUBLIC
        PECore
        PEEvent
        PELayer
        PEWindow
        PEScene
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
        Platform/OpenGL/OpenGLFramebuffer.h
        Platform/OpenGL/OpenGLFramebuffer.cpp
        Platform/OpenGL/OpenGLShader.h
        Platform/OpenGL/OpenGLShader.cpp
        Platform/OpenGL/OpenGLBuffer.h
        Platform/OpenGL/OpenGLBuffer.cpp
        Platform/OpenGL/OpenGLVertexArray.h
        Platform/OpenGL/OpenGLVertexArray.cpp
        Platform/OpenGL/OpenGLUniformBuffer.h
        Platform/OpenGL/OpenGLUniformBuffer.cpp
        Platform/OpenGL/OpenGLTexture2D.h
        Platform/OpenGL/OpenGLTexture2D.cpp
)

if(APPLE)
    target_sources(PEPlatform PRIVATE
        Platform/Metal/MetalContext.mm
        Platform/Metal/MetalContext.h
        Platform/Metal/MetalRendererAPI.mm
        Platform/Metal/MetalRendererAPI.h
    )
endif()
target_link_libraries(PEPlatform
        spdlog
        glfw
        glad
        stbimage
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
        PERendering
        PEEvent
        PECamera
        PEInput
)

if (PROENGINE_ENABLE_EDITOR)
    target_link_libraries(ProEngine INTERFACE PEEditor)
endif ()

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
