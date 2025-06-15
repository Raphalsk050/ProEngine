#pragma once

#include "Core/Camera/Camera.h"
#include "Core/Camera/Camera3D.h"
#include "Core/Camera/Camera3DController.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Texture.h"
#include "Core/Scene/Components.h"
#include "glad/glad.h"
#include <glm.hpp>

namespace ProEngine
{
    // Forward declaration do InstancedRenderer existente
    class InstancedRenderer;

    class EarlyDepthTestManager
    {
    public:
        static void Initialize()
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            glDepthRange(0.0, 1.0);
            glClearDepth(1.0);
            PENGINE_CORE_INFO("Early Depth Test initialized");
        }

        static void BeginFrame()
        {
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }

        static void BeginDepthPrePass()
        {
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
            PENGINE_CORE_TRACE("Starting depth pre-pass");
        }

        static void EndDepthPrePass()
        {
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            PENGINE_CORE_TRACE("Depth pre-pass complete");
        }

        static void BeginColorPass()
        {
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            PENGINE_CORE_TRACE("Starting color pass");
        }

        static void EndColorPass()
        {
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
            PENGINE_CORE_TRACE("Color pass complete");
        }

        static void ConfigureForOpaqueObjects()
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
            glDisable(GL_BLEND);
        }

        static void ConfigureForTransparentObjects()
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LESS);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        static void ConfigureForSkybox()
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
        }

        static void DebugDepthBuffer()
        {
            GLboolean depthTest, depthMask;
            GLint depthFunc;
            GLfloat depthRange[2];

            glGetBooleanv(GL_DEPTH_TEST, &depthTest);
            glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
            glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
            glGetFloatv(GL_DEPTH_RANGE, depthRange);

#ifdef FENGINE_SHADER_DEBUG
            PENGINE_CORE_INFO("=== DEPTH BUFFER STATUS ===");
            PENGINE_CORE_INFO("Depth Test: {}",
                              depthTest ? "ENABLED" : "DISABLED");
            PENGINE_CORE_INFO("Depth Mask: {}",
                              depthMask ? "WRITE" : "READ_ONLY");
            PENGINE_CORE_INFO("Depth Func: {}", GetDepthFuncName(depthFunc));
            PENGINE_CORE_INFO("Depth Range: [{:.3f}, {:.3f}]", depthRange[0],
                              depthRange[1]);
#endif
        }

    private:
        static const char* GetDepthFuncName(GLint func)
        {
            switch (func)
            {
            case GL_NEVER:
                return "NEVER";
            case GL_LESS:
                return "LESS";
            case GL_EQUAL:
                return "EQUAL";
            case GL_LEQUAL:
                return "LEQUAL";
            case GL_GREATER:
                return "GREATER";
            case GL_NOTEQUAL:
                return "NOTEQUAL";
            case GL_GEQUAL:
                return "GEQUAL";
            case GL_ALWAYS:
                return "ALWAYS";
            default:
                return "UNKNOWN";
            }
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

        struct RenderItem
        {
            glm::mat4 Transform;
            Ref<Mesh> MeshPtr;
            Ref<Material> MaterialPtr;
            glm::vec4 Color;
            int EntityID;

            // Para compatibilidade com diferentes tipos de renderização
            enum class Type { Mesh, Cube, Sphere } ItemType = Type::Mesh;
        };

        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera,
                               const glm::mat4& transform);
        static void BeginScene(const Camera3D& camera);
        static void BeginScene(const Camera3DController& cameraController);
        static void EndScene();

        static void StartBatch();
        static void Flush();
        static void NextBatch();

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
                             int entityID = -1);

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

        static void SetInstancingThreshold(uint32_t threshold);
        static uint32_t GetInstancingThreshold();
        static void EnableAutoInstancing(bool enable);
        static bool IsAutoInstancingEnabled();

        static void ResetStats();
        static Statistics GetStats();

        // Estatísticas específicas de culling (mantidas)
        static uint32_t GetTotalMeshCount();
        static uint32_t GetVisibleMeshCount();
        static uint32_t GetCulledMeshCount();
        static float GetCullingEfficiency();

        // Novas estatísticas de instancing
        static float GetInstancingEfficiency();
        static uint32_t GetInstancedObjectCount();
        static uint32_t GetIndividualObjectCount();

        static void DebugCulling();
        static void DebugInstancing();
        static void RecalculateEntityBounds(int entityID);
        static void ClearCullingData();

        static void PreparePrimitives();

    private:
        static void SubmitRenderItem(const RenderItem& item);
        static void ProcessBatches();
        static void RenderInstancedBatch(const std::vector<RenderItem>& items);
        static void RenderIndividualItem(const RenderItem& item);

        // Helpers para agrupamento
        static bool ShouldUseInstancing(const std::vector<RenderItem>& items);
        static std::string GetMeshKey(Ref<Mesh> mesh, Ref<Material> material);

        // Função helper para culling centralizado (mantida)
        friend bool PerformCulling(int entityID, const glm::mat4& transform,
                                   float* outBoundingRadius);

        // Função interna de renderização (modificada para usar o novo sistema)
        friend void DrawMeshInternal(const glm::mat4& transform, Ref<Mesh> mesh,
                                     Ref<Material> material, int entityID);
    };
} // namespace ForgeEngine
