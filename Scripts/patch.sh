 (cd "$(git rev-parse --show-toplevel)" && git apply --3way <<'EOF'
diff --git a/ProEngine/Assets/Shaders/Line3D.glsl b/ProEngine/Assets/Shaders/Line3D.glsl
index 501351df3b5cedc4d3786b1125843eb995a6ac91..40136fe12ad6001f1841cec2a4bcb8186011e821 100644
--- a/ProEngine/Assets/Shaders/Line3D.glsl
+++ b/ProEngine/Assets/Shaders/Line3D.glsl
@@ -1,47 +1,47 @@
 #type vertex
 #version 410 core

 layout(location = 0) in vec3 a_Position;
 layout(location = 1) in vec4 a_Color;
 layout(location = 2) in int a_EntityID;

-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
 };

 struct VertexOutput
 {
     vec4 Color;
 };

 layout(location = 0) out VertexOutput Output;
-layout(location = 1) out flat int v_EntityID;
+layout(location = 1) flat out int v_EntityID;

 void main()
 {
     Output.Color = a_Color;
     v_EntityID = a_EntityID;
     gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
 }

 #type fragment
 #version 410 core

 layout(location = 0) out vec4 o_Color;
 layout(location = 1) out int o_EntityID;

 struct VertexOutput
 {
     vec4 Color;
 };

 layout(location = 0) in VertexOutput Input;
-layout(location = 1) in flat int v_EntityID;
+layout(location = 1) flat in int v_EntityID;

 void main()
 {
     o_Color = Input.Color;
     o_EntityID = v_EntityID;
 }
\ No newline at end of file
diff --git a/ProEngine/Assets/Shaders/Renderer3D_DepthOnly.glsl b/ProEngine/Assets/Shaders/Renderer3D_DepthOnly.glsl
index 19f2242b9ec6100dd3d0c7889e154c82a48c8ca5..e719624b142622ffbe244f38b4baa5d148604827 100644
--- a/ProEngine/Assets/Shaders/Renderer3D_DepthOnly.glsl
+++ b/ProEngine/Assets/Shaders/Renderer3D_DepthOnly.glsl
@@ -1,27 +1,27 @@
 #type vertex
 #version 410 core

 layout(location = 0) in vec3 a_Position;

-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
     float _padding;
 };

 uniform mat4 u_Transform;

 void main()
 {
     gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
 }

 #type fragment
 #version 410 core

 void main()
 {
     // Shader vazio - só queremos escrever depth
     // O OpenGL automaticamente escreve gl_FragDepth
 }
\ No newline at end of file
diff --git a/ProEngine/Assets/Shaders/Renderer3D_Line.glsl b/ProEngine/Assets/Shaders/Renderer3D_Line.glsl
index 501351df3b5cedc4d3786b1125843eb995a6ac91..40136fe12ad6001f1841cec2a4bcb8186011e821 100644
--- a/ProEngine/Assets/Shaders/Renderer3D_Line.glsl
+++ b/ProEngine/Assets/Shaders/Renderer3D_Line.glsl
@@ -1,47 +1,47 @@
 #type vertex
 #version 410 core

 layout(location = 0) in vec3 a_Position;
 layout(location = 1) in vec4 a_Color;
 layout(location = 2) in int a_EntityID;

-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
 };

 struct VertexOutput
 {
     vec4 Color;
 };

 layout(location = 0) out VertexOutput Output;
-layout(location = 1) out flat int v_EntityID;
+layout(location = 1) flat out int v_EntityID;

 void main()
 {
     Output.Color = a_Color;
     v_EntityID = a_EntityID;
     gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
 }

 #type fragment
 #version 410 core

 layout(location = 0) out vec4 o_Color;
 layout(location = 1) out int o_EntityID;

 struct VertexOutput
 {
     vec4 Color;
 };

 layout(location = 0) in VertexOutput Input;
-layout(location = 1) in flat int v_EntityID;
+layout(location = 1) flat in int v_EntityID;

 void main()
 {
     o_Color = Input.Color;
     o_EntityID = v_EntityID;
 }
\ No newline at end of file
diff --git a/ProEngine/Assets/Shaders/Renderer3D_Mesh.glsl b/ProEngine/Assets/Shaders/Renderer3D_Mesh.glsl
index 409496ee523984c246af25544cf6ae1c209be45b..94d7d6b0d3b8471c8ac62f3b7aaa5b94c6f1f81e 100644
--- a/ProEngine/Assets/Shaders/Renderer3D_Mesh.glsl
+++ b/ProEngine/Assets/Shaders/Renderer3D_Mesh.glsl
@@ -1,110 +1,109 @@

 #type vertex
 #version 410 core

 // Inputs do vertex buffer
 layout(location = 0) in vec3 a_Position;
 layout(location = 1) in vec3 a_Normal;
 layout(location = 2) in vec3 a_Tangent;
 layout(location = 3) in vec2 a_TexCoord;

 // Uniform buffers
-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
     float _padding;
 };

-layout(std140, binding = 1) uniform Light
+layout(std140) uniform Light
 {
     vec3 u_PointLightPosition;
     float u_PointLightIntensity;
     vec3 u_AmbientLightColor;
     float u_AmbientLightIntensity;
 };

 // Uniforms individuais
 uniform mat4 u_Transform;

 // Outputs para o fragment shader
 layout(location = 0) out vec3 v_WorldPos;
 layout(location = 1) out vec3 v_Normal;
 layout(location = 2) out vec2 v_TexCoord;

 void main()
 {
     vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
     v_WorldPos = worldPos.xyz;

     v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;

     v_TexCoord = a_TexCoord;

     gl_Position = u_ViewProjection * worldPos;
 }

 #type fragment
 #version 410 core

 // Inputs do vertex shader
-layout(early_fragment_tests) in;
 layout(location = 0) in vec3 v_WorldPos;
 layout(location = 1) in vec3 v_Normal;
 layout(location = 2) in vec2 v_TexCoord;

 // Output
 layout(location = 0) out vec4 o_Color;

 // Uniform buffers
-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
     float _padding;
 };

-layout(std140, binding = 1) uniform Light
+layout(std140) uniform Light
 {
     vec3 u_PointLightPosition;
     float u_PointLightIntensity;
     vec3 u_AmbientLightColor;
     float u_AmbientLightIntensity;
 };

 // Material uniforms
 uniform vec4 u_MaterialAlbedoColor;
 uniform float u_MaterialMetallic;
 uniform float u_MaterialRoughness;
 uniform int u_EntityID;

 // Texture samplers
-layout(binding = 0) uniform sampler2D u_AlbedoMap;
-layout(binding = 1) uniform sampler2D u_NormalMap;
-layout(binding = 2) uniform sampler2D u_MetallicMap;
-layout(binding = 3) uniform sampler2D u_RoughnessMap;
+uniform sampler2D u_AlbedoMap;
+uniform sampler2D u_NormalMap;
+uniform sampler2D u_MetallicMap;
+uniform sampler2D u_RoughnessMap;

 void main()
 {
     // Sample material properties
     vec4 albedoSample = texture(u_AlbedoMap, v_TexCoord);
     vec4 finalAlbedo = albedoSample * u_MaterialAlbedoColor;

     // Normalize the normal
     vec3 normal = normalize(v_Normal);

     // Simple directional lighting
     vec3 lightDir = normalize(u_PointLightPosition - v_WorldPos);
     float NdotL = max(dot(normal, lightDir), 0.0);

     // Calculate lighting - CORRIGIDO: garantir que todos são vec3
     vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
     vec3 diffuse = vec3(NdotL) * vec3(u_PointLightIntensity); // Corrigido: explicit vec3 cast

     // Combine lighting with material
     vec3 lighting = ambient + diffuse;
     vec3 finalColor = finalAlbedo.rgb * lighting;

     // Output final color
     o_Color = vec4(finalColor, finalAlbedo.a);
 }
\ No newline at end of file
diff --git a/ProEngine/Assets/Shaders/Renderer3D_Wireframe.glsl b/ProEngine/Assets/Shaders/Renderer3D_Wireframe.glsl
index 5cc4662c61a5a56f09120184f759715e9a7ed481..89c47b66d5924d93dd39f5f6444378b5cb0be2b7 100644
--- a/ProEngine/Assets/Shaders/Renderer3D_Wireframe.glsl
+++ b/ProEngine/Assets/Shaders/Renderer3D_Wireframe.glsl
@@ -1,46 +1,45 @@
 #type vertex
 #version 410 core
 layout(location = 0) in vec3 a_Position;
 layout(location = 1) in vec3 a_Normal;
 layout(location = 2) in vec3 a_Tangent;
 layout(location = 3) in vec2 a_TexCoord;

-layout(std140, binding = 0) uniform Camera
+layout(std140) uniform Camera
 {
     mat4 u_ViewProjection;
     vec3 u_CameraPosition;
     float _padding;
 };

 uniform mat4 u_Transform;
 uniform int u_EntityID; // ← ADICIONADO: Uniform para entity ID

-layout(location = 0) out flat int v_EntityID;
+layout(location = 0) flat out int v_EntityID;

 void main()
 {
     vec4 worldPosition = u_Transform * vec4(a_Position, 1.0);
     gl_Position = u_ViewProjection * worldPosition;

     v_EntityID = u_EntityID;
 }

 #type fragment
 #version 410 core
-layout(early_fragment_tests) in;
 layout(location = 0) out vec4 o_Color;
 layout(location = 1) out int o_EntityID;

-layout(location = 0) in flat int v_EntityID;
+layout(location = 0) flat in int v_EntityID;

 // Material uniform buffer
 uniform vec4 u_Color;

 void main()
 {
     vec4 finalColor = u_Color;

     finalColor = vec4(1.0, 0.0, 1.0, 1.0);
     o_Color = finalColor;
     o_EntityID = v_EntityID;
 }
\ No newline at end of file
diff --git a/ProEngine/CMakeLists.txt b/ProEngine/CMakeLists.txt
index a61d7714b07212cf4a098fc1d88c7d26f771ebfe..38f8ed651e3db7c8829d6bf0b1e18bc5aaed0e80 100644
--- a/ProEngine/CMakeLists.txt
+++ b/ProEngine/CMakeLists.txt
@@ -139,50 +139,51 @@ engine_add_library(PERendering
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
+        PEPlatform
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
@@ -292,50 +293,51 @@ target_include_directories(PECamera PUBLIC
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
+            PEScene
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
@@ -358,49 +360,49 @@ engine_add_library(PEPlatform
         Platform/OpenGL/OpenGLVertexArray.cpp
         Platform/OpenGL/OpenGLUniformBuffer.h
         Platform/OpenGL/OpenGLUniformBuffer.cpp
         Platform/OpenGL/OpenGLTexture2D.h
         Platform/OpenGL/OpenGLTexture2D.cpp
 )
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
-        PEPlatform
         PERendering
+        PEPlatform
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
diff --git a/ProEngine/Config.h b/ProEngine/Config.h
index 2ba23eba25b832a391456e63d322d591d6074eb3..17c74c3c4d8d8ed0f8e33485ec349a3c69fcdd01 100644
--- a/ProEngine/Config.h
+++ b/ProEngine/Config.h
@@ -2,63 +2,63 @@
 #include <memory>

 // this flag serves to show the shader's possible compilation errors and warnings
 // #define PROENGINE_SHADER_DEBUG

 // to controls the render debug
 // #define PROENGINE_RENDER_DEBUG

 // to enable the culling debug
 // #define PROENGINE_CULLING_DEBUG

 // to debug the frustum information
 // #define PROENGINE_DEBUG_FRUSTUM

 // to debug key input
 // #define PROENGINE_DEBUG_INPUT_KEYS

 // to enable the runtime editor
 // #define PROENGINE_ENABLE_EDITOR

 // to debug the layers
 // #define PROENGINE_DEBUG_LAYERS

 #ifdef DEBUG
 #if defined(PROENGINE_PLATFORM_WINDOWS)
-    #define PROENGINE_DEBUGBREAK() __debugbreak()
+    #define PENGINE_DEBUGBREAK() __debugbreak()
 #elif defined(PROENGINE_PLATFORM_LINUX)
     #include <signal.h>
-    #define PROENGINE_DEBUGBREAK() raise(SIGTRAP)
+    #define PENGINE_DEBUGBREAK() raise(SIGTRAP)
 #elif defined(PROENGINE_PLATFORM_APPLE)
-#include <signal.h>
-#define PENGINE_DEBUGBREAK() raise(SIGTRAP)
+    #include <signal.h>
+    #define PENGINE_DEBUGBREAK() raise(SIGTRAP)
 #else
 #error "Platform doesn't support debugbreak yet!"
 #endif

 #else
-#define PROENGINE_DEBUGBREAK()
+#define PENGINE_DEBUGBREAK()
 #endif

 #define PENGINE_EXPAND_MACRO(x) x
 #define PENGINE_STRINGIFY_MACRO(x) #x

 #define BIT(x) (1 << x)

 #define PENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

 namespace ProEngine
 {
     template <typename T>
     using Scope = std::unique_ptr<T>;

     template <typename T, typename... Args>
     constexpr Scope<T> CreateScope(Args&&... args)
     {
         return std::make_unique<T>(std::forward<Args>(args)...);
     }

     template <typename T>
     using Ref = std::shared_ptr<T>;

     template <typename T, typename... Args>
     constexpr Ref<T> CreateRef(Args&&... args)
diff --git a/ProEngine/Core/Camera/Camera3DController.h b/ProEngine/Core/Camera/Camera3DController.h
index 7f2f1167d74ead46bf3cde720582f6314d97a0fc..809f135757e6ca4aa53fd253a0414b5b1b243691 100644
--- a/ProEngine/Core/Camera/Camera3DController.h
+++ b/ProEngine/Core/Camera/Camera3DController.h
@@ -1,29 +1,29 @@
 #pragma once

 #include "Core/Camera/Camera3D.h"
-#include "Core/TimeStep.h"
+#include "Core/Timestep.h"
 #include "Core/Event/WindowApplicationEvent.h"
 #include "Core/Event/MouseEvent.h"
 #include "Core/Event/KeyEvent.h"

 namespace ProEngine {

 class Camera3DController
 {
 public:
     enum class ControlMode {
         Fly,     // Free-flying camera
         Orbit,   // Orbiting around a target
         FirstPerson // First person camera (no roll)
     };

     Camera3DController(float aspectRatio, ControlMode mode = ControlMode::Fly);

     void OnUpdate(Timestep ts);
     void OnEvent(Event& e);
     void OnResize(float width, float height);

     // Camera access
     Camera3D& GetCamera() { return m_Camera; }
     const Camera3D& GetCamera() const { return m_Camera; }

diff --git a/ProEngine/Core/Editor/ImguiLayer.h b/ProEngine/Core/Editor/ImguiLayer.h
index 7ad1059162af843241faf63e2a6a55de5a7bdf04..1be5532354fabf841c3b65c9eb0d4c14a8a71c38 100644
--- a/ProEngine/Core/Editor/ImguiLayer.h
+++ b/ProEngine/Core/Editor/ImguiLayer.h
@@ -1,27 +1,27 @@
 #pragma once
-#include "Core/TimeStep.h"
+#include "Core/Timestep.h"
 #include "Core/Event/Event.h"
 #include "Core/Layer/Layer.h"

 namespace ProEngine {
     class ImGuiLayer : public Layer {
     public:
         ImGuiLayer();
         ~ImGuiLayer() = default;

         virtual void OnAttach() override;
         void OnUpdate(Timestep ts) override;
         virtual void OnDetach() override;
         virtual void OnEvent(Event& e) override;

         void Begin();
         void End();

         void BlockEvents(bool block) { block_events_ = block; }

         void SetDarkThemeColors();

         void ShowMetricsWindow();

         uint32_t GetActiveWidgetID() const;

diff --git a/ProEngine/Core/Renderer/Renderer3D.cpp b/ProEngine/Core/Renderer/Renderer3D.cpp
index 1ddcf5f984c7f6bf6bc53bfeec3a3654dfe0b56d..753044c179adc51338db4f3ab4f1de37498af6b6 100644
--- a/ProEngine/Core/Renderer/Renderer3D.cpp
+++ b/ProEngine/Core/Renderer/Renderer3D.cpp
@@ -1,76 +1,61 @@
 #include "Core/Renderer/Renderer3D.h"
 #include "Config.h"
-#include "Core/Renderer/InstancedRenderer.h"
 #include "Core/Renderer/RenderCommand.h"
 #include "Core/Renderer/Shader.h"
 #include "Core/Renderer/UniformBuffer.h"
 #include "Core/Renderer/VertexArray.h"
 #include "glad/glad.h"
 #include <gtc/matrix_transform.hpp>
 #include <gtc/type_ptr.hpp>
 #include <unordered_map>

 namespace ProEngine
 {
     struct MeshVertex
     {
         glm::vec3 Position;
         glm::vec3 Normal;
         glm::vec3 Tangent;
         glm::vec2 TexCoord;
         glm::vec4 Color;
         int EntityID;
     };

     struct LineVertex3D
     {
         glm::vec3 Position;
         glm::vec4 Color;
         int EntityID;
     };

     struct Renderer3DData
     {
         static constexpr uint32_t MaxVertices = 100000;
         static constexpr uint32_t MaxIndices = 200000;
         static constexpr uint32_t MaxTextureSlots = 32;

-        // Instancing configuration
-        uint32_t InstancingThreshold
-            = 3; // Minimum objects to enable instancing
-        bool AutoInstancingEnabled = true;
-
-        // Collection of render items for batching
-        std::vector<Renderer3D::RenderItem> RenderQueue;
-
-        // Mapping mesh+material -> items for efficient batching
-        std::unordered_map<std::string, std::vector<Renderer3D::RenderItem>>
-        MeshBatches;
-
-        // Reference to the InstancedRenderer
-        std::unique_ptr<InstancedRenderer> InstanceRenderer;

         // Active camera for frustum culling
         const Camera3D* ActiveCamera = nullptr;

         // Entity culling info (stores bounding volumes)
         struct EntityCullingData
         {
             glm::vec3 BoundingBoxMin;
             glm::vec3 BoundingBoxMax;
             float BoundingSphereRadius;
             bool WasVisible = true;
         };

         std::unordered_map<int, EntityCullingData> EntityCullingInfo;

         // Tracks visible and total entities for culling stats
         uint32_t VisibleMeshCount = 0;
         uint32_t TotalMeshCount = 0;

         // Statistics of the last frame
         Renderer3D::Statistics LastFrameStats;

         // Meshes
         Ref<Shader> MeshShader;
         Ref<Shader> WireframeShader;
@@ -134,62 +119,61 @@ namespace ProEngine
     static Renderer3DData s_Data;

     // Creates a transform matrix from position, scale and Euler rotation (XYZ
     // order)
     glm::mat4 CreateTransformMatrix(const glm::vec3& position,
                                     const glm::vec3& scale,
                                     const glm::vec3& rotation)
     {
         glm::mat4 transformMatrix = glm::mat4(1.0f);
         transformMatrix = glm::translate(transformMatrix, position);
         transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.z),
                                       glm::vec3(0.0f, 0.0f, 1.0f));
         transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.y),
                                       glm::vec3(0.0f, 1.0f, 0.0f));
         transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.x),
                                       glm::vec3(1.0f, 0.0f, 0.0f));
         transformMatrix = glm::scale(transformMatrix, scale);
         return transformMatrix;
     }

     // Performs frustum culling for an entity based on its transform and
     // bounding sphere
     bool PerformCulling(int entityID, const glm::mat4& transform,
                         float* outBoundingRadius = nullptr)
     {
-        if (!s_Data.ActiveCamera || entityID < 0)
+        if (!s_Data.ActiveCamera)
+        {
+            PENGINE_CORE_ASSERT(false, "No Active Camera!")
+            PENGINE_CORE_ERROR("No Active Camera!");
+            return true;
+        }
+
+        // Allow negative entity IDs for objects not associated with a scene
+        // entity. Skip culling in this case but continue rendering.
+        if (entityID < 0)
         {
-            if (s_Data.ActiveCamera == nullptr)
-            {
-                PENGINE_CORE_ASSERT(false, "No Active Camera!")
-                PENGINE_CORE_ERROR("No Active Camera!");
-            }
-            if (entityID < 0)
-            {
-                PENGINE_CORE_ASSERT(false, "EntityID is invalid!")
-                PENGINE_CORE_ERROR("EntityID is invalid!");
-            }
             return true;
         }

         auto& cullingData = s_Data.EntityCullingInfo[entityID];

         // Calculate bounding sphere radius if not set yet
         if (cullingData.BoundingSphereRadius == 0.0f)
         {
             glm::vec3 scale;
             scale.x = glm::length(glm::vec3(transform[0]));
             scale.y = glm::length(glm::vec3(transform[1]));
             scale.z = glm::length(glm::vec3(transform[2]));
             float maxScale = glm::max(glm::max(scale.x, scale.y), scale.z);
             cullingData.BoundingSphereRadius
                 = maxScale * 0.866f; // ~sqrt(3)/2 for a cube
         }

         s_Data.TotalMeshCount++;

         bool isVisible = Renderer3D::IsEntityVisible(
             entityID, transform, cullingData.BoundingSphereRadius);

         if (isVisible)
         {
             s_Data.VisibleMeshCount++;
@@ -233,92 +217,82 @@ namespace ProEngine
         else
             s_Data.WhiteTexture->Bind(3);

         // Switch between wireframe and standard shader
         if (s_Data.WireframeMode)
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
             s_Data.WireframeShader->Bind();
             s_Data.WireframeShader->SetMat4("u_Transform", transform);
             s_Data.WireframeShader->SetFloat4("u_Color",
                                               material->GetAlbedoColor());
             s_Data.WireframeShader->SetInt("u_EntityID", entityID);
         }
         else
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
             s_Data.MeshShader->Bind();
             s_Data.MeshShader->SetMat4("u_Transform", transform);
             s_Data.MeshShader->SetFloat4("u_MaterialAlbedoColor",
                                          material->GetAlbedoColor());
             s_Data.MeshShader->SetFloat("u_MaterialMetallic",
                                         material->GetMetallic());
             s_Data.MeshShader->SetFloat("u_MaterialRoughness",
                                         material->GetRoughness());
             s_Data.MeshShader->SetInt("u_AlbedoMap", 0);
-            s_Data.MeshShader->SetFloat4("u_NormalMap",
-                                         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
-            s_Data.MeshShader->SetFloat4("u_MetallicMap",
-                                         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
-            s_Data.MeshShader->SetFloat4("u_RoughnessMap",
-                                         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
+            s_Data.MeshShader->SetInt("u_NormalMap", 1);
+            s_Data.MeshShader->SetInt("u_MetallicMap", 2);
+            s_Data.MeshShader->SetInt("u_RoughnessMap", 3);
             s_Data.MeshShader->SetInt("u_EntityID", entityID);
         }

         // Bind VAO and issue draw call
         mesh->GetVertexArray()->Bind();
         RenderCommand::DrawIndexed(mesh->GetVertexArray(),
                                    mesh->GetIndexCount());

         s_Data.Stats.DrawCalls++;
         s_Data.Stats.IndividualDrawCalls++;
         s_Data.Stats.VertexCount += mesh->GetVertexCount();
         s_Data.Stats.IndexCount += mesh->GetIndexCount();
     }

     void Renderer3D::Init()
     {
         PENGINE_PROFILE_FUNCTION();
         EarlyDepthTestManager::Initialize();

         s_Data.MeshShader = Shader::Create(
             "../ProEngine/Assets/Shaders/Renderer3D_Mesh.glsl");
         s_Data.WireframeShader = Shader::Create(
             "../ProEngine/Assets/Shaders/Renderer3D_Wireframe.glsl");
         s_Data.LineShader = Shader::Create(
             "../ProEngine/Assets/Shaders/Renderer3D_Line.glsl");

         if (s_Data.MeshShader == nullptr)
             PENGINE_CORE_CRITICAL("Mesh shader not found");

-        // Initialize InstancedRenderer
-        s_Data.InstanceRenderer = std::make_unique<InstancedRenderer>();
-        s_Data.InstanceRenderer->Init();
-
-        PENGINE_CORE_INFO(
-            "Instanced rendering system initialized with threshold: {}",
-            s_Data.InstancingThreshold);

         // Create line rendering resources
         s_Data.LineVertexArray = VertexArray::Create();
         s_Data.LineVertexBuffer = VertexBuffer::Create(
             Renderer3DData::MaxVertices * sizeof(LineVertex3D));
         s_Data.LineVertexBuffer->SetLayout({
             {ShaderDataType::Float3, "a_Position"},
             {ShaderDataType::Float4, "a_Color"},
             {ShaderDataType::Int, "a_EntityID"},
         });
         s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
         s_Data.LineVertexBufferBase
             = new LineVertex3D[Renderer3DData::MaxVertices];

         // Create white texture
         s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
         uint32_t whiteTextureData = 0xffffffff;
         s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
         s_Data.TextureSlots[0] = s_Data.WhiteTexture;

         // Create primitive meshes
         s_Data.CubeMesh = Mesh::CreateCube(1.0f);
         s_Data.SphereMesh = Mesh::CreateSphere(0.5f, 16, 16);

         PENGINE_CORE_INFO("Primitive meshes created:");
@@ -335,57 +309,50 @@ namespace ProEngine
         s_Data.DefaultMaterial->SetRoughness(0.5f);
         s_Data.DefaultMaterial->SetMetallic(0.0f);

         // Create uniform buffers
         s_Data.CameraUniformBuffer
             = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
         s_Data.LightUniformBuffer
             = UniformBuffer::Create(sizeof(Renderer3DData::LightData), 1);

         // Set default lighting data
         s_Data.LightBuffer.PointLightPosition = s_Data.PointLightPosition;
         s_Data.LightBuffer.PointLightIntensity = 1.0f;
         s_Data.LightBuffer.AmbientLightColor = s_Data.AmbientLightColor;
         s_Data.LightBuffer.AmbientLightIntensity = s_Data.AmbientLightIntensity;
         s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer,
                                            sizeof(Renderer3DData::LightData));

         PENGINE_CORE_INFO("Renderer3D initialized successfully with hybrid "
             "instancing system");
     }

     void Renderer3D::Shutdown()
     {
         PENGINE_PROFILE_FUNCTION();

-        // Shutdown instanced renderer
-        if (s_Data.InstanceRenderer)
-        {
-            s_Data.InstanceRenderer->Shutdown();
-            s_Data.InstanceRenderer.reset();
-        }
-
         delete[] s_Data.LineVertexBufferBase;
     }

     void Renderer3D::BeginScene(const Camera& camera,
                                 const glm::mat4& transform)
     {
         PENGINE_PROFILE_FUNCTION();
         EarlyDepthTestManager::BeginFrame();

         s_Data.CameraBuffer.ViewProjection
             = camera.GetProjection() * glm::inverse(transform);
         s_Data.CameraBuffer.CameraPosition
             = glm::vec3(glm::inverse(transform)[3]);
         s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer,
                                             sizeof(Renderer3DData::CameraData));

         // Update lighting data
         s_Data.LightBuffer.PointLightPosition = s_Data.PointLightPosition;
         s_Data.LightBuffer.AmbientLightColor = s_Data.AmbientLightColor;
         s_Data.LightBuffer.AmbientLightIntensity = s_Data.AmbientLightIntensity;
         s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer,
                                            sizeof(Renderer3DData::LightData));

         s_Data.ActiveCamera = nullptr;
         StartBatch();
@@ -404,228 +371,89 @@ namespace ProEngine
         // Update lighting data
         s_Data.LightBuffer.PointLightPosition = s_Data.PointLightPosition;
         s_Data.LightBuffer.AmbientLightColor = s_Data.AmbientLightColor;
         s_Data.LightBuffer.AmbientLightIntensity = s_Data.AmbientLightIntensity;
         s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer,
                                            sizeof(Renderer3DData::LightData));

         s_Data.ActiveCamera = &camera;
         StartBatch();
     }

     void Renderer3D::BeginScene(const Camera3DController& cameraController)
     {
         BeginScene(cameraController.GetCamera());
     }

     void Renderer3D::EndScene()
     {
         PENGINE_PROFILE_FUNCTION();

         s_Data.Stats.MeshCount = s_Data.TotalMeshCount;
         s_Data.Stats.VisibleMeshCount = s_Data.VisibleMeshCount;
         s_Data.Stats.CulledMeshCount
             = s_Data.TotalMeshCount - s_Data.VisibleMeshCount;

-        ProcessBatches();
-
         Flush();
-
-        if (s_Data.Stats.TotalInstances > 0)
-        {
-            uint32_t drawCallsWithoutInstancing
-                = s_Data.Stats.IndividualDrawCalls
-                + s_Data.Stats.TotalInstances;
-            uint32_t actualDrawCalls = s_Data.Stats.IndividualDrawCalls
-                + s_Data.Stats.InstancedDrawCalls;
-            s_Data.Stats.InstancingEfficiency
-                = (float)(drawCallsWithoutInstancing - actualDrawCalls)
-                / (float)drawCallsWithoutInstancing * 100.0f;
-        }
     }

     void Renderer3D::StartBatch()
     {
-        // Clear previous frame's data
-        s_Data.RenderQueue.clear();
-        s_Data.MeshBatches.clear();
-
         // Reset line rendering
         s_Data.LineVertexCount = 0;
         s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
         s_Data.TextureSlotIndex = 1;

         // Reset counters
         s_Data.VisibleMeshCount = 0;
         s_Data.TotalMeshCount = 0;

-        // Reset instanced renderer stats
-        if (s_Data.InstanceRenderer) { s_Data.InstanceRenderer->ResetStats(); }
     }

-    void Renderer3D::ProcessBatches()
-    {
-        PENGINE_PROFILE_FUNCTION();
-
-        if (!s_Data.AutoInstancingEnabled)
-        {
-            // If auto instancing is disabled, render everything individually
-            for (const auto& item : s_Data.RenderQueue)
-            {
-                RenderIndividualItem(item);
-            }
-            return;
-        }
-
-        // Group items by mesh and material
-        for (const auto& item : s_Data.RenderQueue)
-        {
-            std::string meshKey = GetMeshKey(item.MeshPtr, item.MaterialPtr);
-            s_Data.MeshBatches[meshKey].push_back(item);
-        }
-
-        // Process each group
-        for (const auto& [meshKey, items] : s_Data.MeshBatches)
-        {
-            if (ShouldUseInstancing(items)) { RenderInstancedBatch(items); }
-            else
-            {
-                for (const auto& item : items) { RenderIndividualItem(item); }
-            }
-        }
-    }
-
-    void Renderer3D::RenderInstancedBatch(const std::vector<RenderItem>& items)
-    {
-        PENGINE_PROFILE_FUNCTION();
-
-        if (items.empty() || !s_Data.InstanceRenderer) return;
-
-        // Prepare data for instancing
-        std::vector<glm::mat4> transforms;
-        std::vector<glm::vec4> colors;
-        std::vector<int> entityIDs;
-
-        transforms.reserve(items.size());
-        colors.reserve(items.size());
-        entityIDs.reserve(items.size());
-
-        for (const auto& item : items)
-        {
-            transforms.push_back(item.Transform);
-            colors.push_back(item.Color);
-            entityIDs.push_back(item.EntityID);
-        }
-
-        // Use InstancedRenderer
-        s_Data.InstanceRenderer->DrawInstancedMesh(transforms, items[0].MeshPtr,
-                                                   colors, entityIDs);
-
-        // Update statistics
-        s_Data.Stats.InstancedDrawCalls++;
-        s_Data.Stats.TotalInstances += items.size();
-        s_Data.Stats.InstancedObjects += items.size();
-
-#ifdef FENGINE_SHADER_DEBUG
-        FENGINE_CORE_TRACE(
-            "Rendered {} instances of mesh {} in single draw call",
-            items.size(),
-            GetMeshKey(items[0].MeshPtr, items[0].MaterialPtr));
-#endif
-
-    }
-
-    void Renderer3D::RenderIndividualItem(const RenderItem& item)
-    {
-        PENGINE_PROFILE_FUNCTION();
-
-        Ref<Material> material = item.MaterialPtr;
-        if (!material)
-        {
-            // Use default material with specified color
-            s_Data.DefaultMaterial->SetAlbedoColor(item.Color);
-            material = s_Data.DefaultMaterial;
-        }
-
-        DrawMeshInternal(item.Transform, item.MeshPtr, material, item.EntityID);
-        s_Data.Stats.IndividualObjects++;
-    }
-
-    bool Renderer3D::ShouldUseInstancing(const std::vector<RenderItem>& items)
-    {
-        return s_Data.AutoInstancingEnabled
-            && items.size() >= s_Data.InstancingThreshold
-            && items.size() <= s_Data.InstanceRenderer->GetMaxInstances();
-    }
-
-    std::string Renderer3D::GetMeshKey(Ref<Mesh> mesh, Ref<Material> material)
-    {
-        // Use mesh and material pointers as unique key
-        uintptr_t meshPtr = reinterpret_cast<uintptr_t>(mesh.get());
-        uintptr_t matPtr = reinterpret_cast<uintptr_t>(material.get());
-        return std::to_string(meshPtr) + "_" + std::to_string(matPtr);
-    }
-
-    void Renderer3D::SubmitRenderItem(const RenderItem& item)
-    {
-        s_Data.RenderQueue.push_back(item);
-    }

     void Renderer3D::DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                               const glm::vec4& color, int entityID)
     {
         PENGINE_PROFILE_FUNCTION();

         if (!PerformCulling(entityID, transform)) { return; }

-        RenderItem item;
-        item.Transform = transform;
-        item.MeshPtr = mesh;
-        item.MaterialPtr = nullptr; // Use color directly
-        item.Color = color;
-        item.EntityID = entityID;
-        item.ItemType = RenderItem::Type::Mesh;
-
-        SubmitRenderItem(item);
+        s_Data.DefaultMaterial->SetAlbedoColor(color);
+        DrawMeshInternal(transform, mesh, s_Data.DefaultMaterial, entityID);
+        s_Data.Stats.IndividualObjects++;
     }

     void Renderer3D::DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                               Ref<Material> material, int entityID)
     {
         PENGINE_PROFILE_FUNCTION();

         if (!PerformCulling(entityID, transform)) { return; }

-        RenderItem item;
-        item.Transform = transform;
-        item.MeshPtr = mesh;
-        item.MaterialPtr = material;
-        item.Color = material ? material->GetAlbedoColor() : glm::vec4(1.0f);
-        item.EntityID = entityID;
-        item.ItemType = RenderItem::Type::Mesh;
-
-        SubmitRenderItem(item);
+        DrawMeshInternal(transform, mesh, material ? material : s_Data.DefaultMaterial,
+                        entityID);
+        s_Data.Stats.IndividualObjects++;
     }

     void Renderer3D::DrawMesh(const glm::vec3& position, const glm::vec3& scale,
                               const glm::vec3& rotation, Ref<Mesh> mesh,
                               const glm::vec4& color, int entityID)
     {
         glm::mat4 transform = CreateTransformMatrix(position, scale, rotation);
         DrawMesh(transform, mesh, color, entityID);
     }

     void Renderer3D::DrawMesh(const glm::vec3& position, const glm::vec3& scale,
                               const glm::vec3& rotation, Ref<Mesh> mesh,
                               Ref<Material> material, int entityID)
     {
         glm::mat4 transform = CreateTransformMatrix(position, scale, rotation);
         DrawMesh(transform, mesh, material, entityID);
     }

     void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size,
                               const glm::vec4& color, int entityID)
     {
         glm::mat4 transform
             = CreateTransformMatrix(position, size, glm::vec3(0.0f));
         DrawCube(transform, color, entityID);
     }
@@ -755,117 +583,90 @@ namespace ProEngine
         p6 = transform * glm::vec4(p6, 1.0f);
         p7 = transform * glm::vec4(p7, 1.0f);

         // Bottom face
         DrawLine3D(p0, p1, color, entityID);
         DrawLine3D(p1, p2, color, entityID);
         DrawLine3D(p2, p3, color, entityID);
         DrawLine3D(p3, p0, color, entityID);

         // Top face
         DrawLine3D(p4, p5, color, entityID);
         DrawLine3D(p5, p6, color, entityID);
         DrawLine3D(p6, p7, color, entityID);
         DrawLine3D(p7, p4, color, entityID);

         // Connecting edges
         DrawLine3D(p0, p4, color, entityID);
         DrawLine3D(p1, p5, color, entityID);
         DrawLine3D(p2, p6, color, entityID);
         DrawLine3D(p3, p7, color, entityID);
     }

     void Renderer3D::DrawModel(const glm::mat4& transform,
                                ModelRendererComponent& src, int entityID)
     {
-        if (!src.Model) return;
+        if (!src.ModelRef) return;

-        for (const auto& mesh : src.Model->GetMeshes())
+        for (const auto& mesh : src.ModelRef->GetMeshes())
         {
             if (src.OverrideMaterial)
                 DrawMesh(transform, mesh, src.OverrideMaterial, entityID);
             else if (mesh->GetMaterial())
                 DrawMesh(transform, mesh, mesh->GetMaterial(), entityID);
             else
                 DrawMesh(transform, mesh, s_Data.DefaultMaterial, entityID);
         }
     }

     void Renderer3D::Flush()
     {
         PENGINE_PROFILE_FUNCTION();

         if (s_Data.LineVertexCount)
         {
             uint32_t dataSize
                 = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr
                     - (uint8_t*)s_Data.LineVertexBufferBase);
             s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase,
                                              dataSize);

             s_Data.LineShader->Bind();
             RenderCommand::SetLineWidth(s_Data.LineWidth);
             RenderCommand::DrawLines(s_Data.LineVertexArray,
                                      s_Data.LineVertexCount);
             s_Data.Stats.DrawCalls++;
         }
     }

-    void Renderer3D::NextBatch()
-    {
-        Flush();
-        StartBatch();
-    }

     void Renderer3D::PreparePrimitives()
     {
         s_Data.CubeMesh = Mesh::CreateCube();
         s_Data.SphereMesh = Mesh::CreateSphere();
     }

-    void Renderer3D::SetInstancingThreshold(uint32_t threshold)
-    {
-        s_Data.InstancingThreshold = threshold;
-        PENGINE_CORE_INFO("Instancing threshold set to: {}", threshold);
-    }
-
-    uint32_t Renderer3D::GetInstancingThreshold()
-    {
-        return s_Data.InstancingThreshold;
-    }
-
-    void Renderer3D::EnableAutoInstancing(bool enable)
-    {
-        s_Data.AutoInstancingEnabled = enable;
-        PENGINE_CORE_INFO("Auto instancing {}",
-                          enable ? "enabled" : "disabled");
-    }
-
-    bool Renderer3D::IsAutoInstancingEnabled()
-    {
-        return s_Data.AutoInstancingEnabled;
-    }

     bool Renderer3D::IsPointVisible(const glm::vec3& point)
     {
         if (s_Data.ActiveCamera)
         {
             return s_Data.ActiveCamera->PointInFrustum(point);
         }
         return true;
     }

     bool Renderer3D::IsSphereVisible(const glm::vec3& center, float radius)
     {
         if (s_Data.ActiveCamera)
         {
             return s_Data.ActiveCamera->SphereInFrustum(center, radius);
         }
         return true;
     }

     bool Renderer3D::IsAABBVisible(const glm::vec3& min, const glm::vec3& max)
     {
         if (s_Data.ActiveCamera)
         {
             return s_Data.ActiveCamera->AABBInFrustum(min, max);
         }
@@ -921,102 +722,68 @@ namespace ProEngine
         return s_Data.Stats;
     }

     uint32_t Renderer3D::GetTotalMeshCount()
     {
         return s_Data.TotalMeshCount;
     }

     uint32_t Renderer3D::GetVisibleMeshCount()
     {
         return s_Data.VisibleMeshCount;
     }

     uint32_t Renderer3D::GetCulledMeshCount()
     {
         return s_Data.TotalMeshCount - s_Data.VisibleMeshCount;
     }

     float Renderer3D::GetCullingEfficiency()
     {
         if (s_Data.TotalMeshCount == 0) return 0.0f;
         return (float)(s_Data.TotalMeshCount - s_Data.VisibleMeshCount)
             / (float)s_Data.TotalMeshCount * 100.0f;
     }

-    float Renderer3D::GetInstancingEfficiency()
-    {
-        return s_Data.Stats.InstancingEfficiency;
-    }
-
-    uint32_t Renderer3D::GetInstancedObjectCount()
-    {
-        return s_Data.Stats.InstancedObjects;
-    }
-
-    uint32_t Renderer3D::GetIndividualObjectCount()
-    {
-        return s_Data.Stats.IndividualObjects;
-    }

     void Renderer3D::DebugCulling()
     {
 #ifdef FENGINE_CULLING_DEBUG
         FENGINE_CORE_INFO("=== CULLING DEBUG ===");
         FENGINE_CORE_INFO("Total meshes: {}", s_Data.TotalMeshCount);
         FENGINE_CORE_INFO("Visible meshes: {}", s_Data.VisibleMeshCount);
         FENGINE_CORE_INFO("Culled meshes: {}",
                           s_Data.TotalMeshCount - s_Data.VisibleMeshCount);
         FENGINE_CORE_INFO("Active camera: {}",
                           s_Data.ActiveCamera ? "YES" : "NO");

         if (s_Data.ActiveCamera) {
             glm::vec3 camPos = s_Data.ActiveCamera->GetPosition();
             FENGINE_CORE_INFO("Camera position: ({:.2f}, {:.2f}, {:.2f})",
                               camPos.x, camPos.y, camPos.z);
         }

         int count = 0;
         for (const auto& [entityID, cullingData]: s_Data.EntityCullingInfo) {
             if (count++ > 5) break;
             FENGINE_CORE_INFO("Entity {}: radius={:.2f}, visible={}", entityID,
                               cullingData.BoundingSphereRadius,
                               cullingData.WasVisible ? "YES" : "NO");
         }
 #endif
     }

-    void Renderer3D::DebugInstancing()
-    {
-#ifdef FENGINE_CULLING_DEBUG
-        FENGINE_CORE_INFO("=== INSTANCING DEBUG ===");
-        FENGINE_CORE_INFO("Auto instancing: {}", s_Data.AutoInstancingEnabled
-                          ? "ENABLED"
-                          : "DISABLED");
-        FENGINE_CORE_INFO("Instancing threshold: {}",
-                          s_Data.InstancingThreshold);
-        FENGINE_CORE_INFO("Render queue size: {}", s_Data.RenderQueue.size());
-        FENGINE_CORE_INFO("Mesh batches: {}", s_Data.MeshBatches.size());
-        FENGINE_CORE_INFO("Instanced draw calls: {}",
-                          s_Data.Stats.InstancedDrawCalls);
-        FENGINE_CORE_INFO("Individual draw calls: {}",
-                          s_Data.Stats.IndividualDrawCalls);
-        FENGINE_CORE_INFO("Total instances: {}", s_Data.Stats.TotalInstances);
-        FENGINE_CORE_INFO("Instancing efficiency: {:.2f}%",
-                          s_Data.Stats.InstancingEfficiency);
-#endif
-    }

     void Renderer3D::RecalculateEntityBounds(int entityID)
     {
         auto it = s_Data.EntityCullingInfo.find(entityID);
         if (it != s_Data.EntityCullingInfo.end())
         {
             it->second.BoundingSphereRadius = 0.0f;
         }
     }

     void Renderer3D::ClearCullingData()
     {
         s_Data.EntityCullingInfo.clear();
     }
 } // namespace ForgeEngine
diff --git a/ProEngine/Core/Renderer/Renderer3D.h b/ProEngine/Core/Renderer/Renderer3D.h
index aa5cd315286b02c8dbe1bf7fd7960f477585b77c..f21ab290cfd96a94b736bcc8b1242e6c4b36f22d 100644
--- a/ProEngine/Core/Renderer/Renderer3D.h
+++ b/ProEngine/Core/Renderer/Renderer3D.h
@@ -140,74 +140,61 @@ namespace ProEngine
         }
     };

     class Renderer3D
     {
     public:
         struct Statistics
         {
             uint32_t DrawCalls = 0;
             uint32_t MeshCount = 0;
             uint32_t VisibleMeshCount = 0;
             uint32_t CulledMeshCount = 0;
             uint32_t VertexCount = 0;
             uint32_t IndexCount = 0;

             // Novas estatísticas para instancing
             uint32_t InstancedDrawCalls = 0;
             uint32_t IndividualDrawCalls = 0;
             uint32_t TotalInstances = 0;
             uint32_t InstancedObjects = 0;
             uint32_t IndividualObjects = 0;
             float InstancingEfficiency
                 = 0.0f; // Porcentagem de redução de draw calls
         };

-        struct RenderItem
-        {
-            glm::mat4 Transform;
-            Ref<Mesh> MeshPtr;
-            Ref<Material> MaterialPtr;
-            glm::vec4 Color;
-            int EntityID;
-
-            // Para compatibilidade com diferentes tipos de renderização
-            enum class Type { Mesh, Cube, Sphere } ItemType = Type::Mesh;
-        };

         static void Init();
         static void Shutdown();

         static void BeginScene(const Camera& camera,
                                const glm::mat4& transform);
         static void BeginScene(const Camera3D& camera);
         static void BeginScene(const Camera3DController& cameraController);
         static void EndScene();

-        static void StartBatch();
         static void Flush();
-        static void NextBatch();

         static bool IsPointVisible(const glm::vec3& point);
         static bool IsSphereVisible(const glm::vec3& center, float radius);
         static bool IsAABBVisible(const glm::vec3& min, const glm::vec3& max);
         static bool IsEntityVisible(int entityID, const glm::mat4& transform,
                                     float boundingSphereRadius);

         static void DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                              const glm::vec4& color, int entityID = -1);
         static void DrawMesh(const glm::vec3& position, const glm::vec3& scale,
                              const glm::vec3& rotation, Ref<Mesh> mesh,
                              const glm::vec4& color, int entityID = -1);
         static void DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh,
                              Ref<Material> material, int entityID = -1);
         static void DrawMesh(const glm::vec3& position, const glm::vec3& scale,
                              const glm::vec3& rotation, Ref<Mesh> mesh,
                              Ref<Material> material, int entityID = -1);

         static void DrawCube(const glm::vec3& position, const glm::vec3& size,
                              const glm::vec4& color, int entityID = -1);
         static void DrawCube(const glm::vec3& position, const glm::vec3& size,
                              Ref<Material> material, int entityID = -1);
         static void DrawCube(const glm::mat4& transform, const glm::vec4& color,
                              int entityID = -1);
         static void DrawCube(const glm::mat4& transform, Ref<Material> material,
@@ -216,70 +203,53 @@ namespace ProEngine
         static void DrawSphere(const glm::vec3& position, float radius,
                                const glm::vec4& color, int entityID = -1);
         static void DrawSphere(const glm::vec3& position, float radius,
                                Ref<Material> material, int entityID = -1);
         static void DrawSphere(const glm::mat4& transform,
                                const glm::vec4& color, int entityID = -1);
         static void DrawSphere(const glm::mat4& transform,
                                Ref<Material> material, int entityID = -1);

         static void DrawLine3D(const glm::vec3& p0, const glm::vec3& p1,
                                const glm::vec4& color, int entityID = -1);
         static void DrawBox(const glm::vec3& position, const glm::vec3& size,
                             const glm::vec4& color, int entityID = -1);
         static void DrawBox(const glm::mat4& transform, const glm::vec4& color,
                             int entityID = -1);

         static void DrawModel(const glm::mat4& transform,
                               ModelRendererComponent& src, int entityID = -1);

         static void SetPointLightPosition(const glm::vec3& position);
         static void SetAmbientLight(const glm::vec3& color, float intensity);

         static void EnableWireframe(bool enable);
         static bool IsWireframeEnabled();

-        static void SetInstancingThreshold(uint32_t threshold);
-        static uint32_t GetInstancingThreshold();
-        static void EnableAutoInstancing(bool enable);
-        static bool IsAutoInstancingEnabled();

         static void ResetStats();
         static Statistics GetStats();

         // Estatísticas específicas de culling (mantidas)
         static uint32_t GetTotalMeshCount();
         static uint32_t GetVisibleMeshCount();
         static uint32_t GetCulledMeshCount();
         static float GetCullingEfficiency();

-        // Novas estatísticas de instancing
-        static float GetInstancingEfficiency();
-        static uint32_t GetInstancedObjectCount();
-        static uint32_t GetIndividualObjectCount();
-
         static void DebugCulling();
-        static void DebugInstancing();
         static void RecalculateEntityBounds(int entityID);
         static void ClearCullingData();

         static void PreparePrimitives();

     private:
-        static void SubmitRenderItem(const RenderItem& item);
-        static void ProcessBatches();
-        static void RenderInstancedBatch(const std::vector<RenderItem>& items);
-        static void RenderIndividualItem(const RenderItem& item);
-
-        // Helpers para agrupamento
-        static bool ShouldUseInstancing(const std::vector<RenderItem>& items);
-        static std::string GetMeshKey(Ref<Mesh> mesh, Ref<Material> material);
+        static void StartBatch();

         // Função helper para culling centralizado (mantida)
         friend bool PerformCulling(int entityID, const glm::mat4& transform,
                                    float* outBoundingRadius);

         // Função interna de renderização (modificada para usar o novo sistema)
         friend void DrawMeshInternal(const glm::mat4& transform, Ref<Mesh> mesh,
                                      Ref<Material> material, int entityID);
     };
 } // namespace ForgeEngine
diff --git a/ProEngine/Core/Scene/Components.h b/ProEngine/Core/Scene/Components.h
index 69cb6932d558d7315acf9dae7f001e95f9a33017..f01895128ad9aebad59932a846c3e3fcc1f37a2c 100644
--- a/ProEngine/Core/Scene/Components.h
+++ b/ProEngine/Core/Scene/Components.h
@@ -19,37 +19,50 @@ namespace ProEngine
     struct TransformComponent
     {
         glm::vec3 position{0.0f};
         glm::vec3 rotation{0.0f};
         glm::vec3 scale{1.0f};

         entt::entity parent{entt::null};
         entt::entity first_child{entt::null};
         entt::entity next_sibling{entt::null};
         entt::entity prev_sibling{entt::null};

         glm::mat4 LocalMatrix() const
         {
             glm::mat4 mat(1.0f);
             mat = glm::translate(mat, position);
             mat = glm::rotate(mat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
             mat = glm::rotate(mat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
             mat = glm::rotate(mat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
             mat = glm::scale(mat, scale);
             return mat;
         }
     };

     struct ModelRendererComponent
     {
-        Ref<Model> Model;
+        Ref<Model> ModelRef;
         Ref<Material> OverrideMaterial;

         ModelRendererComponent() = default;
         ModelRendererComponent(const ModelRendererComponent&) = default;

-        ModelRendererComponent(const Ref<ProEngine::Model>& model)
-            : Model(model)
+        ModelRendererComponent(const Ref<Model>& model)
+            : ModelRef(model)
+        {
+        }
+    };
+
+    struct MeshRendererComponent
+    {
+        Ref<Mesh> MeshRef;
+        Ref<Material> MaterialRef;
+
+        MeshRendererComponent() = default;
+        MeshRendererComponent(const Ref<Mesh>& mesh,
+                              const Ref<Material>& material = nullptr)
+            : MeshRef(mesh), MaterialRef(material)
         {
         }
     };
 } // namespace ProEngine
diff --git a/ProEngine/Core/Scene/Scene.cpp b/ProEngine/Core/Scene/Scene.cpp
index 42094f16c1239c6126a99277bd8d52b605eff8c3..a1b9cc71d00cb102f3293e00894d993b1b42d2a1 100644
--- a/ProEngine/Core/Scene/Scene.cpp
+++ b/ProEngine/Core/Scene/Scene.cpp
@@ -1,43 +1,64 @@
 #include "Core/Scene/Scene.h"
 #include "Core/Scene/EntityHandle.h"
 #include "Components.h"
+#include "Core/Renderer/Renderer3D.h"


 namespace ProEngine
 {
     EntityHandle Scene::CreateEntity(const std::string& name)
     {
         entt::entity e = registry_.create();
         EntityHandle handle{e, this};
         registry_.emplace<TransformComponent>(e);
         registry_.emplace<TagComponent>(e, TagComponent{name});
         return handle;
     }

     void Scene::OnUpdate(Timestep ts)
     {
+        auto meshView
+            = registry_.view<TransformComponent, MeshRendererComponent>();
+        for (auto entity : meshView)
+        {
+            auto& tr = meshView.get<TransformComponent>(entity);
+            auto& mr = meshView.get<MeshRendererComponent>(entity);
+            glm::mat4 world = GetWorldMatrix(entity);
+            Renderer3D::DrawMesh(world, mr.MeshRef, mr.MaterialRef,
+                                static_cast<int>(entity));
+        }
+
+        auto modelView
+            = registry_.view<TransformComponent, ModelRendererComponent>();
+        for (auto entity : modelView)
+        {
+            auto& tr = modelView.get<TransformComponent>(entity);
+            auto& mr = modelView.get<ModelRendererComponent>(entity);
+            glm::mat4 world = GetWorldMatrix(entity);
+            Renderer3D::DrawModel(world, mr, static_cast<int>(entity));
+        }
     }

     void Scene::LateUpdate(Timestep ts)
     {
     }

     void Scene::OnEvent(Event& e)
     {
     }

     std::vector<EntityHandle*> Scene::GetAllEntities()
     {
         std::vector<EntityHandle*> handles = std::vector<EntityHandle*>();
         auto view = GetRegistry().view<TransformComponent, TagComponent>();

         for (auto entity : view)
         {
             handles.push_back(new EntityHandle(entity, this));
         }
         return handles;
     }

     glm::mat4 Scene::GetWorldMatrix(entt::entity entity) const
     {
         const auto& tr = registry_.get<TransformComponent>(entity);
diff --git a/ProEngine/Platform/OpenGL/OpenGLBuffer.cpp b/ProEngine/Platform/OpenGL/OpenGLBuffer.cpp
index c85f8268a62bfc6177dd6603f1dc3a1b42de3a33..f13342f2c9ec45531374e809d1e2812128e854f6 100644
--- a/ProEngine/Platform/OpenGL/OpenGLBuffer.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLBuffer.cpp
@@ -1,90 +1,90 @@
 #include "PEPCH.h"
 #include "OpenGLBuffer.h"
 #include <glad/glad.h>

 namespace ProEngine {

 	/////////////////////////////////////////////////////////////////////////////
 	// VertexBuffer /////////////////////////////////////////////////////////////
 	/////////////////////////////////////////////////////////////////////////////

 	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
 	{
 		PENGINE_PROFILE_FUNCTION();

-		glCreateBuffers(1, &m_RendererID);
+                glGenBuffers(1, &m_RendererID);
 		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
 		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
 	}

 	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
 	{
 		PENGINE_PROFILE_FUNCTION();

-		glCreateBuffers(1, &m_RendererID);
+                glGenBuffers(1, &m_RendererID);
 		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
 		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
 	}

 	OpenGLVertexBuffer::~OpenGLVertexBuffer()
 	{
 		PENGINE_PROFILE_FUNCTION();

 		glDeleteBuffers(1, &m_RendererID);
 	}

 	void OpenGLVertexBuffer::Bind() const
 	{
 		PENGINE_PROFILE_FUNCTION();

 		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
 	}

 	void OpenGLVertexBuffer::Unbind() const
 	{
 		PENGINE_PROFILE_FUNCTION();

 		glBindBuffer(GL_ARRAY_BUFFER, 0);
 	}

 	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
 	{
 		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
 		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
 	}

 	/////////////////////////////////////////////////////////////////////////////
 	// IndexBuffer //////////////////////////////////////////////////////////////
 	/////////////////////////////////////////////////////////////////////////////

 	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
 		: m_Count(count)
 	{
 		PENGINE_PROFILE_FUNCTION();

-		glCreateBuffers(1, &m_RendererID);
+                glGenBuffers(1, &m_RendererID);

 		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
 		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
 		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
 		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
 	}

 	OpenGLIndexBuffer::~OpenGLIndexBuffer()
 	{
 		PENGINE_PROFILE_FUNCTION();

 		glDeleteBuffers(1, &m_RendererID);
 	}

 	void OpenGLIndexBuffer::Bind() const
 	{
 		PENGINE_PROFILE_FUNCTION();

 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
 	}

 	void OpenGLIndexBuffer::Unbind() const
 	{
 		PENGINE_PROFILE_FUNCTION();

diff --git a/ProEngine/Platform/OpenGL/OpenGLFramebuffer.cpp b/ProEngine/Platform/OpenGL/OpenGLFramebuffer.cpp
index a1cac7813903a79f65ed62d3b3197c0c07714aff..bb043a071ed7fa873324c5f227b3a0e8d1972059 100644
--- a/ProEngine/Platform/OpenGL/OpenGLFramebuffer.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLFramebuffer.cpp
@@ -4,60 +4,61 @@

 namespace ProEngine
 {
     OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
     {
         specification_ = spec;
         Invalidate();
     }

     OpenGLFramebuffer::~OpenGLFramebuffer()
     {
         glDeleteFramebuffers(1, &renderer_id_);
         glDeleteTextures(1, &color_attachment_);
         glDeleteRenderbuffers(1, &depth_attachment_);
     }

     void OpenGLFramebuffer::Invalidate()
     {
         if (renderer_id_ > -1)
         {
             glDeleteFramebuffers(1, &renderer_id_);
             glDeleteTextures(1, &color_attachment_);
             glDeleteRenderbuffers(1, &depth_attachment_);
         }

-        glCreateFramebuffers(1, &renderer_id_);
+        glGenFramebuffers(1, &renderer_id_);
         glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

-        glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment_);
-        glTextureStorage2D(color_attachment_, 1, GL_RGBA8, specification_.Width, specification_.Height);
-        glTextureParameteri(color_attachment_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
-        glTextureParameteri(color_attachment_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
+        glGenTextures(1, &color_attachment_);
+        glBindTexture(GL_TEXTURE_2D, color_attachment_);
+        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification_.Width, specification_.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
+        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
+        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

-        glCreateRenderbuffers(1, &depth_attachment_);
+        glGenRenderbuffers(1, &depth_attachment_);
         glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment_);
         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, specification_.Width, specification_.Height);
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_attachment_);

         PENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

         glBindFramebuffer(GL_FRAMEBUFFER, 0);
     }

     void OpenGLFramebuffer::Bind() const
     {
         glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
         glViewport(0, 0, specification_.Width, specification_.Height);
     }

     void OpenGLFramebuffer::Unbind() const
     {
         glBindFramebuffer(GL_FRAMEBUFFER, 0);
     }

     void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
     {
         if (width == 0 || height == 0)
             return;

diff --git a/ProEngine/Platform/OpenGL/OpenGLShader.cpp b/ProEngine/Platform/OpenGL/OpenGLShader.cpp
index cff121f13d0d5637c0bf688621d1ad86ac7e98c5..cafa7a8ebebe2b4069ff113579328cb82010a28b 100644
--- a/ProEngine/Platform/OpenGL/OpenGLShader.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLShader.cpp
@@ -254,50 +254,59 @@ namespace ProEngine
             glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

             if (maxLength > 0)
             {
                 std::vector<GLchar> infoLog(maxLength);
                 glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
                 PENGINE_CORE_ERROR("Shader linking failed:\n{}", infoLog.data());
             }
             else
             {
                 PENGINE_CORE_ERROR("Shader linking failed with no error message");
             }

             glDeleteProgram(program);

             // Limpar shaders individuais
             for (auto id : shaderIDs)
             {
                 glDeleteShader(id);
             }
             return;
         }

         PENGINE_CORE_TRACE("Shader program linked successfully");

+        // Bind uniform blocks manually for GL 4.1
+        GLuint cameraBlock = glGetUniformBlockIndex(program, "Camera");
+        if (cameraBlock != GL_INVALID_INDEX)
+            glUniformBlockBinding(program, cameraBlock, 0);
+
+        GLuint lightBlock = glGetUniformBlockIndex(program, "Light");
+        if (lightBlock != GL_INVALID_INDEX)
+            glUniformBlockBinding(program, lightBlock, 1);
+
         // Limpar shaders individuais (já não são necessários após linking)
         for (auto id : shaderIDs)
         {
             glDetachShader(program, id);
             glDeleteShader(id);
         }

         m_RendererID = program;
     }

     void OpenGLShader::Bind() const
     {
         PENGINE_PROFILE_FUNCTION();
         glUseProgram(m_RendererID);
     }

     void OpenGLShader::Unbind() const
     {
         PENGINE_PROFILE_FUNCTION();
         glUseProgram(0);
     }

     void OpenGLShader::SetInt(const std::string& name, int value)
     {
         PENGINE_PROFILE_FUNCTION();
diff --git a/ProEngine/Platform/OpenGL/OpenGLTexture2D.cpp b/ProEngine/Platform/OpenGL/OpenGLTexture2D.cpp
index f4c9d21fe6b28c871082ac8b8de6daf72674584b..65f60e2d6bb5ff3d114f50e40a179e96a326c29e 100644
--- a/ProEngine/Platform/OpenGL/OpenGLTexture2D.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLTexture2D.cpp
@@ -23,105 +23,108 @@ static GLenum ForgeEngineImageFormatToGLDataFormat(ImageFormat format) {

 static GLenum ForgeEngineImageFormatToGLInternalFormat(ImageFormat format) {
   switch (format) {
     case ImageFormat::RGB8:
       return GL_RGB8;
     case ImageFormat::RGBA8:
       return GL_RGBA8;
   }

   PENGINE_CORE_ASSERT(false);
   return 0;
 }

 }  // namespace Utils

 OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
     : m_Specification(specification),
       m_Width(m_Specification.Width),
       m_Height(m_Specification.Height) {
   PENGINE_PROFILE_FUNCTION();

   m_InternalFormat =
       Utils::ForgeEngineImageFormatToGLInternalFormat(m_Specification.Format);
   m_DataFormat = Utils::ForgeEngineImageFormatToGLDataFormat(m_Specification.Format);

-  glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
-  glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
+  glGenTextures(1, &m_RendererID);
+  glBindTexture(GL_TEXTURE_2D, m_RendererID);
+  glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0,
+               m_DataFormat, GL_UNSIGNED_BYTE, nullptr);

-  glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
-  glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
+  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
+  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

-  glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
-  glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
+  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
+  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 }

 OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
   PENGINE_PROFILE_FUNCTION();

   int width, height, channels;
   stbi_set_flip_vertically_on_load(1);
   stbi_uc* data = nullptr;
   {
     PENGINE_PROFILE_SCOPE(
         "stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
     data = stbi_load(path.c_str(), &width, &height, &channels, 0);
   }

   if (data) {
     m_IsLoaded = true;

     m_Width = width;
     m_Height = height;

     GLenum internalFormat = 0, dataFormat = 0;
     if (channels == 4) {
       internalFormat = GL_RGBA8;
       dataFormat = GL_RGBA;
     } else if (channels == 3) {
       internalFormat = GL_RGB8;
       dataFormat = GL_RGB;
     }

     m_InternalFormat = internalFormat;
     m_DataFormat = dataFormat;

     PENGINE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

-    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
-    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
+    glGenTextures(1, &m_RendererID);
+    glBindTexture(GL_TEXTURE_2D, m_RendererID);
+    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0,
+                 dataFormat, GL_UNSIGNED_BYTE, data);

-    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
-    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

-    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
-    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
-
-    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat,
-                        GL_UNSIGNED_BYTE, data);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
+    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     stbi_image_free(data);
   }
 }

 OpenGLTexture2D::~OpenGLTexture2D() {
   PENGINE_PROFILE_FUNCTION();

   glDeleteTextures(1, &m_RendererID);
 }

 void OpenGLTexture2D::SetData(void* data, uint32_t size) {
   PENGINE_PROFILE_FUNCTION();

   uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
   PENGINE_CORE_ASSERT(size == m_Width * m_Height * bpp,
                  "Data must be entire texture!");
-  glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat,
-                      GL_UNSIGNED_BYTE, data);
+  glBindTexture(GL_TEXTURE_2D, m_RendererID);
+  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat,
+                  GL_UNSIGNED_BYTE, data);
 }

 void OpenGLTexture2D::Bind(uint32_t slot) const {
   PENGINE_PROFILE_FUNCTION();

-  glBindTextureUnit(slot, m_RendererID);
+  glActiveTexture(GL_TEXTURE0 + slot);
+  glBindTexture(GL_TEXTURE_2D, m_RendererID);
 }
 }  // namespace ForgeEngine
\ No newline at end of file
diff --git a/ProEngine/Platform/OpenGL/OpenGLUniformBuffer.cpp b/ProEngine/Platform/OpenGL/OpenGLUniformBuffer.cpp
index c3b47ccce4b48605bf6b282095af82db40bae726..325394e9adfeabb72f6f9379ff4a105ea3c53346 100644
--- a/ProEngine/Platform/OpenGL/OpenGLUniformBuffer.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLUniformBuffer.cpp
@@ -1,26 +1,28 @@
 #include "PEPCH.h"
 #include "Platform/OpenGL/OpenGLUniformBuffer.h"

 #include <glad/glad.h>

 namespace ProEngine {

 OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
 {
-  glCreateBuffers(1, &m_RendererID);
-  glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
+  glGenBuffers(1, &m_RendererID);
+  glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
+  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
   glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
 }

 OpenGLUniformBuffer::~OpenGLUniformBuffer()
 {
   glDeleteBuffers(1, &m_RendererID);
 }


 void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
 {
-  glNamedBufferSubData(m_RendererID, offset, size, data);
+  glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
+  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
 }

 }
\ No newline at end of file
diff --git a/ProEngine/Platform/OpenGL/OpenGLVertexArray.cpp b/ProEngine/Platform/OpenGL/OpenGLVertexArray.cpp
index 060d47355678a7afdc475c83598bcbcf8118d402..6bdcc6ed2e5427da8e05dff31ac0dc8a1c8272e4 100644
--- a/ProEngine/Platform/OpenGL/OpenGLVertexArray.cpp
+++ b/ProEngine/Platform/OpenGL/OpenGLVertexArray.cpp
@@ -23,51 +23,51 @@ static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
       return GL_FLOAT;
     case ShaderDataType::Int:
       return GL_INT;
     case ShaderDataType::Int2:
       return GL_INT;
     case ShaderDataType::Int3:
       return GL_INT;
     case ShaderDataType::Int4:
       return GL_INT;
     case ShaderDataType::Bool:
       return GL_BOOL;
   }

   PENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
   return 0;
 }

 OpenGLVertexArray::OpenGLVertexArray() {
   PENGINE_PROFILE_FUNCTION();

   const GLubyte* renderer = glGetString(GL_RENDERER);
   const GLubyte* vendor = glGetString(GL_VENDOR);
   const GLubyte* version = glGetString(GL_VERSION);
   const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

-  glCreateVertexArrays(1, &m_RendererID);
+  glGenVertexArrays(1, &m_RendererID);
 }

 OpenGLVertexArray::~OpenGLVertexArray() {
   PENGINE_PROFILE_FUNCTION();

   glDeleteVertexArrays(1, &m_RendererID);
 }

 void OpenGLVertexArray::Bind() const {
   PENGINE_PROFILE_FUNCTION();

   glBindVertexArray(m_RendererID);
 }

 void OpenGLVertexArray::Unbind() const {
   PENGINE_PROFILE_FUNCTION();

   glBindVertexArray(0);
 }

 void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
   PENGINE_PROFILE_FUNCTION();

   PENGINE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                       "Vertex Buffer has no layout!");
diff --git a/Program/CMakeLists.txt b/Program/CMakeLists.txt
index d6d52e6c1f844e2811178811620cec169a405119..c40c6964b8f0cd63dc4386255bd0ea1b168c391a 100644
--- a/Program/CMakeLists.txt
+++ b/Program/CMakeLists.txt
@@ -1,28 +1,28 @@
 project(SampleApp LANGUAGES CXX)

 if(POLICY CMP0065)
     cmake_policy(SET CMP0065 NEW)
 endif()

 if(POLICY CMP0156)
     cmake_policy(SET CMP0156 NEW)
 endif()

 if(POLICY CMP0179)
     cmake_policy(SET CMP0179 NEW)
 endif()


-add_executable(SampleApp Program.cpp SampleLayer.cpp SampleLayer.h)
+add_executable(SampleApp Program.cpp SampleLayer.cpp SampleLayer.h SceneLayer.cpp SceneLayer.h)
 target_link_libraries(SampleApp PUBLIC ProEngine ImGui)
 target_include_directories(SampleApp PUBLIC ${PROJECT_SOURCE_DIR}/ProEngine ${PROJECT_SOURCE_DIR}/ProEngine/ThirdParty/imgui)

 if (CMAKE_VERSION VERSION_GREATER 3.12)
     set_property(TARGET SampleApp PROPERTY CXX_STANDARD 20)
 endif()

 add_custom_command(TARGET SampleApp POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${CMAKE_SOURCE_DIR}/ProEngine/Assets
         $<TARGET_FILE_DIR:SampleApp>/../ProEngine/Assets
 )
\ No newline at end of file
diff --git a/Program/Program.cpp b/Program/Program.cpp
index de3b562ec175d5d5b4a869eff8465432a4f545b8..b14681e4ec437c559a9a4a70ccae609f7d463359 100644
--- a/Program/Program.cpp
+++ b/Program/Program.cpp
@@ -1,24 +1,25 @@
 ﻿#include <ProEngine.h>
 #include "SampleLayer.h"
+#include "SceneLayer.h"

 using namespace std;

 namespace ProEngine {
   class Sample : public Application {
   public:
     explicit Sample(const ApplicationSpecification &spec)
       : Application(spec) {
-      PushLayer(new SampleLayer());
+      PushLayer(new SceneLayer());
     }
   };

   Application *CreateApplication(ApplicationCommandLineArgs args) {
     ApplicationSpecification spec;
     spec.Name = "SampleApp";
     spec.CommandLineArgs = args;
     spec.WindowWidth = 1600;
     spec.WindowHeight = 900;

     return new Sample(spec);
   }
 }
diff --git a/Program/SceneLayer.cpp b/Program/SceneLayer.cpp
new file mode 100644
index 0000000000000000000000000000000000000000..a35f4a971d9662b65c368f6f9b205d8e33efc85d
--- /dev/null
+++ b/Program/SceneLayer.cpp
@@ -0,0 +1,61 @@
+#include "SceneLayer.h"
+#include "Core/Application/Application.h"
+#include "Core/Renderer/RenderCommand.h"
+#include "Core/Renderer/Renderer3D.h"
+#include "Core/Renderer/Mesh.h"
+#include "imgui.h"
+
+namespace ProEngine {
+SceneLayer::SceneLayer() : Layer("SceneLayer") {
+    auto& window = Application::Get().GetWindow();
+    camera_controller_ = Camera3DController(window.GetWidth() / (float)window.GetHeight());
+}
+
+void SceneLayer::OnAttach() {
+    Layer::OnAttach();
+    glEnable(GL_DEPTH_TEST);
+    glDepthFunc(GL_LEQUAL);
+    camera_controller_.SetPosition({0.0f, 0.0f, 3.0f});
+    camera_controller_.SetRotation({0.0f, 0.0f, 0.0f});
+    scene_ = CreateScope<Scene>();
+    cube_entity_ = scene_->CreateEntity("Cube");
+    cube_entity_.AddComponent<MeshRendererComponent>(Mesh::CreateCube());
+    FramebufferSpecification spec;
+    spec.Width = Application::Get().GetWindow().GetWidth();
+    spec.Height = Application::Get().GetWindow().GetHeight();
+    framebuffer_ = Framebuffer::Create(spec);
+    viewport_size_ = {(float)spec.Width, (float)spec.Height};
+    camera_controller_.OnResize(spec.Width, spec.Height);
+}
+
+void SceneLayer::OnDetach() {
+    Layer::OnDetach();
+}
+
+void SceneLayer::OnUpdate(Timestep ts) {
+    Layer::OnUpdate(ts);
+    camera_controller_.OnUpdate(ts);
+    time_ += ts;
+    framebuffer_->Bind();
+    RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
+    RenderCommand::Clear();
+    Renderer3D::BeginScene(camera_controller_.GetCamera());
+    if (scene_)
+        scene_->OnUpdate(ts);
+    Renderer3D::EndScene();
+    framebuffer_->Unbind();
+}
+
+void SceneLayer::OnImGuiRender() {
+    Layer::OnImGuiRender();
+    ImGui::Begin("Viewport");
+    ImVec2 size = ImGui::GetContentRegionAvail();
+    if (viewport_size_.x != size.x || viewport_size_.y != size.y) {
+        framebuffer_->Resize((uint32_t)size.x, (uint32_t)size.y);
+        viewport_size_ = {size.x, size.y};
+        camera_controller_.OnResize((uint32_t)size.x, (uint32_t)size.y);
+    }
+    ImGui::Image((void*)(intptr_t)framebuffer_->GetColorAttachmentRendererID(), size, ImVec2(0,1), ImVec2(1,0));
+    ImGui::End();
+}
+} // namespace ProEngine
diff --git a/Program/SceneLayer.h b/Program/SceneLayer.h
new file mode 100644
index 0000000000000000000000000000000000000000..e9bade324ff6a900845654f55847689b235b2828
--- /dev/null
+++ b/Program/SceneLayer.h
@@ -0,0 +1,25 @@
+#pragma once
+#include "Core/Camera/Camera3DController.h"
+#include "Core/Layer/Layer.h"
+#include "Core/Renderer/Mesh.h"
+#include "Core/Renderer/Framebuffer.h"
+#include "Core/Scene/Scene.h"
+#include "Core/Scene/EntityHandle.h"
+
+namespace ProEngine {
+    class SceneLayer : public Layer {
+    public:
+        SceneLayer();
+        void OnAttach() override;
+        void OnUpdate(Timestep ts) override;
+        void OnImGuiRender() override;
+        void OnDetach() override;
+    private:
+        Camera3DController camera_controller_ = Camera3DController(1.0f);
+        Scope<Scene> scene_;
+        EntityHandle cube_entity_;
+        Ref<Framebuffer> framebuffer_;
+        glm::vec2 viewport_size_ = {0.0f, 0.0f};
+        float time_ = 0.0f;
+    };
+} // namespace ProEngine
diff --git a/build.sh b/build.sh
index 91781a9d3930f642935f34008431ca449c390008..ae5bb2d40070e5f0fc0bef98ac7d93a6470585a0 100755
--- a/build.sh
+++ b/build.sh
@@ -1,19 +1,15 @@
+set -e
 BUILD_DIR="build"

 if [ ! -d "$BUILD_DIR" ]; then
     echo "build directory don't exists! Creating one..."
     mkdir ${BUILD_DIR}
     echo "build directory has been created!"
 fi

-cmake -G Ninja -B ${BUILD_DIR} -Wdev --debug-output --trace -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_POLICY_VERSION_MINIMUM=3.5
+cmake -G Ninja -S . -B ${BUILD_DIR} -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DPROENGINE_ENABLE_EDITOR=ON

 cd ${BUILD_DIR}

 ninja -j 0
 echo "Build complete!"
-
-if [ $? -ne 0 ]; then
-    echo "Build failed!"
-    exit 1
-fi

EOF
)