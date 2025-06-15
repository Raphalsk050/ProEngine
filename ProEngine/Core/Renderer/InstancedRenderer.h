#pragma once

#include "Config.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Material.h"
#include <glm.hpp>
#include <vector>
#include <unordered_map>

namespace ProEngine
{
    struct OptimizedInstanceData
    {
        glm::mat4 Transform;
        glm::vec4 Color;
        glm::vec4 CustomData; // Metallic, Roughness, EntityID, padding
    };

    class InstancedRenderer
    {
    public:
        // Constructor/Destructor
        InstancedRenderer() = default;
        ~InstancedRenderer() = default;

        // Core functionality
        void Init();
        void Shutdown();

        // Main rendering function
        void DrawInstancedMesh(const std::vector<glm::mat4>& transforms,
                               Ref<Mesh> mesh,
                               const std::vector<glm::vec4>& colors,
                               const std::vector<int>& entityIDs);

        // Utility functions
        void ClearCache();
        uint32_t GetMaxInstances() const { return MAX_INSTANCES; }

        // Stats
        struct InstancedStats
        {
            uint32_t TotalInstances = 0;
            uint32_t VisibleInstances = 0;
            uint32_t CachedVAOs = 0;
            uint32_t BufferUpdates = 0;
            uint32_t DrawCalls = 0;
        };

        InstancedStats GetStats() const { return m_Stats; }
        void ResetStats();

        // Material control for compatibility
        void SetDefaultMaterial(Ref<Material> material) { m_DefaultMaterial = material; }

    private:
        static constexpr uint32_t MAX_INSTANCES = 100000;

        // Core rendering components
        Ref<VertexBuffer> m_InstanceBuffer;
        Ref<Shader> m_InstancedShader;

        // Instance data management
        std::vector<OptimizedInstanceData> m_InstanceData;
        bool m_BufferNeedsUpdate = true;

        // VAO cache for different mesh types
        std::unordered_map<Ref<Mesh>, Ref<VertexArray>> m_InstancedVAOs;

        // Material padrão para compatibilidade
        Ref<Material> m_DefaultMaterial;

        // Statistics
        mutable InstancedStats m_Stats;

        // Private helper functions
        void CreateInstancedShader();
        void PrepareInstanceData(const std::vector<glm::mat4>& transforms,
                                 const std::vector<glm::vec4>& colors,
                                 const std::vector<int>& entityIDs);

        Ref<VertexArray> GetOrCreateInstancedVAO(Ref<Mesh> mesh);
        void SetupInstanceAttributes(Ref<VertexArray> vao);
        void RenderInstanced(Ref<VertexArray> vao, Ref<Mesh> mesh, uint32_t instanceCount);
    };

    namespace InstancedRendering
    {
        void Initialize();
        void Shutdown();
        void DrawMesh(const std::vector<glm::mat4>& transforms,
                      Ref<Mesh> mesh,
                      const std::vector<glm::vec4>& colors,
                      const std::vector<int>& entityIDs);
        InstancedRenderer::InstancedStats GetStats();
        void ResetStats();
    }
} // namespace ForgeEngine
