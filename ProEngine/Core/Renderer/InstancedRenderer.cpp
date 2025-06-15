#include "Core/Renderer/InstancedRenderer.h"
#include "Core/Renderer/RenderCommand.h"
#include "glad/glad.h"
#include <gtc/matrix_transform.hpp>
#include "PEPCH.h"

namespace ProEngine
{
    // Forward declaration para função de culling (definida em Renderer3D.cpp)
    bool PerformCulling(int entityID, const glm::mat4& transform, float* outBoundingRadius);

    void InstancedRenderer::Init()
    {
#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Initializing Efficient Instanced Renderer...");
#endif
        CreateInstancedShader();

        m_InstanceBuffer = VertexBuffer::Create(MAX_INSTANCES * sizeof(OptimizedInstanceData));

        if (!m_InstanceBuffer)
        {
            PENGINE_CORE_ERROR("Failed to create instance buffer!");
            return;
        }

        BufferLayout instanceLayout = {
            {ShaderDataType::Mat4, "a_InstanceMatrix"}, // locations 3,4,5,6
            {ShaderDataType::Float4, "a_InstanceColor"}, // location 7
            {ShaderDataType::Float4, "a_InstanceCustomData"} // location 8
        };
        m_InstanceBuffer->SetLayout(instanceLayout);

        m_InstanceData.reserve(MAX_INSTANCES);

        ResetStats();
#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Instanced Renderer initialized successfully. Max instances: {}", MAX_INSTANCES);
#endif
    }

    void InstancedRenderer::Shutdown()
    {
#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Shutting down Instanced Renderer...");
#endif
        // Clear cache
        m_InstancedVAOs.clear();
        m_InstanceData.clear();

        // Reset pointers
        m_InstanceBuffer.reset();
        m_InstancedShader.reset();
#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Instanced Renderer shutdown complete");
#endif
    }

    void InstancedRenderer::DrawInstancedMesh(const std::vector<glm::mat4>& transforms,
                                              Ref<Mesh> mesh,
                                              const std::vector<glm::vec4>& colors,
                                              const std::vector<int>& entityIDs)
    {
        PENGINE_PROFILE_FUNCTION();

        if (transforms.empty() || transforms.size() != colors.size() || transforms.size() != entityIDs.size())
        {
            PENGINE_CORE_ERROR("DrawInstancedMesh: Mismatched input sizes - transforms: {}, colors: {}, entityIDs: {}",
                               transforms.size(), colors.size(), entityIDs.size());
            return;
        }

        if (!mesh || !m_InstanceBuffer || !m_InstancedShader)
        {
            PENGINE_CORE_ERROR("DrawInstancedMesh: Invalid mesh, buffer, or shader!");
            return;
        }

        m_Stats.TotalInstances += transforms.size();

        PrepareInstanceData(transforms, colors, entityIDs);

        if (m_InstanceData.empty())
        {
            return;
        }

        m_Stats.VisibleInstances += m_InstanceData.size();

        Ref<VertexArray> instancedVAO = GetOrCreateInstancedVAO(mesh);

        if (!instancedVAO)
        {
            PENGINE_CORE_ERROR("Failed to create or get instanced VAO!");
            return;
        }

        if (m_BufferNeedsUpdate)
        {
            m_InstanceBuffer->SetData(m_InstanceData.data(),
                                      m_InstanceData.size() * sizeof(OptimizedInstanceData));
            m_BufferNeedsUpdate = false;
            m_Stats.BufferUpdates++;
        }

        RenderInstanced(instancedVAO, mesh, m_InstanceData.size());

        // Track draw calls
        m_Stats.DrawCalls++;
    }

    void InstancedRenderer::CreateInstancedShader()
    {
        std::string vertexCode = R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

layout(location = 4) in mat4 a_InstanceMatrix;     // locations 4,5,6,7
layout(location = 8) in vec4 a_InstanceColor;      // location 8
layout(location = 9) in vec4 a_InstanceCustomData; // location 9

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float padding1;
};

layout(std140, binding = 1) uniform Light
{
    vec3 u_PointLightPosition;
    float u_PointLightIntensity;
    vec3 u_AmbientLightColor;
    float u_AmbientLightIntensity;
};

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_InstanceColor;
out vec4 v_CustomData;

void main()
{
    vec4 worldPos = a_InstanceMatrix * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    v_Normal = mat3(a_InstanceMatrix) * a_Normal;

    v_TexCoord = a_TexCoord;
    v_InstanceColor = a_InstanceColor;
    v_CustomData = a_InstanceCustomData;

    gl_Position = u_ViewProjection * worldPos;
}
)";

        std::string fragmentCode = R"(
#version 450 core

layout(early_fragment_tests) in;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_InstanceColor;
in vec4 v_CustomData;

out vec4 o_Color;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float padding1;
};

layout(std140, binding = 1) uniform Light
{
    vec3 u_PointLightPosition;
    float u_PointLightIntensity;
    vec3 u_AmbientLightColor;
    float u_AmbientLightIntensity;
};

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform int u_Wireframe;

void main()
{
    float metallic = v_CustomData.x;
    float roughness = v_CustomData.y;
    int entityID = int(v_CustomData.z);

    vec4 albedoSample = texture(u_AlbedoMap, v_TexCoord);
    vec4 finalAlbedo = albedoSample * v_InstanceColor;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_PointLightPosition - v_WorldPos);
    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
    vec3 diffuse = vec3(NdotL * u_PointLightIntensity);

    vec3 lighting = ambient + diffuse;
    vec3 finalColor = finalAlbedo.rgb * lighting;

    o_Color = vec4(finalColor, finalAlbedo.a);
}
)";

        try
        {
            m_InstancedShader = Shader::Create("InstancedMesh", vertexCode, fragmentCode);
            if (!m_InstancedShader)
            {
                PENGINE_CORE_ERROR("Failed to create instanced shader!");
            }
            else
            {
#ifdef FENGINE_RENDER_DEBUG
                FENGINE_CORE_INFO("Instanced shader created successfully");
#endif
            }
        }
        catch (const std::exception& e)
        {
            PENGINE_CORE_ERROR("Exception creating instanced shader: {}", e.what());
        }
    }

    void InstancedRenderer::PrepareInstanceData(const std::vector<glm::mat4>& transforms,
                                                const std::vector<glm::vec4>& colors,
                                                const std::vector<int>& entityIDs)
    {
        m_InstanceData.clear();

        for (size_t i = 0; i < transforms.size(); ++i)
        {
            // Use the culling function from Renderer3D
            if (!PerformCulling(entityIDs[i], transforms[i]))
            {
                continue;
            }

            if (m_InstanceData.size() >= MAX_INSTANCES)
            {
                PENGINE_CORE_WARN("Reached maximum instance limit: {}", MAX_INSTANCES);
                break;
            }

            OptimizedInstanceData instance;
            instance.Transform = transforms[i];
            instance.Color = colors[i];
            instance.CustomData = glm::vec4(
                0.0f, // Metallic
                0.5f, // Roughness
                float(entityIDs[i]), // EntityID
                0.0f // Padding
            );

            m_InstanceData.push_back(instance);
        }

        m_BufferNeedsUpdate = true;
    }

    Ref<VertexArray> InstancedRenderer::GetOrCreateInstancedVAO(Ref<Mesh> mesh)
    {
        auto it = m_InstancedVAOs.find(mesh);
        if (it != m_InstancedVAOs.end())
        {
            return it->second;
        }

        Ref<VertexArray> instancedVAO = VertexArray::Create();

        if (!instancedVAO)
        {
            PENGINE_CORE_ERROR("Failed to create instanced VAO!");
            return nullptr;
        }

        const auto& meshVAO = mesh->GetVertexArray();
        if (!meshVAO || meshVAO->GetVertexBuffers().empty())
        {
            PENGINE_CORE_ERROR("Mesh has no vertex buffers!");
            return nullptr;
        }

        instancedVAO->AddVertexBuffer(meshVAO->GetVertexBuffers()[0]);
        instancedVAO->AddVertexBuffer(m_InstanceBuffer);
        instancedVAO->SetIndexBuffer(meshVAO->GetIndexBuffer());

        SetupInstanceAttributes(instancedVAO);

        m_InstancedVAOs[mesh] = instancedVAO;
        m_Stats.CachedVAOs = m_InstancedVAOs.size();

#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Created new instanced VAO for mesh. Total cached: {}", m_Stats.CachedVAOs);
#endif
        return instancedVAO;
    }

    void InstancedRenderer::SetupInstanceAttributes(Ref<VertexArray> vao)
    {
        vao->Bind();
        m_InstanceBuffer->Bind();

        const auto& layout = m_InstanceBuffer->GetLayout();
        uint32_t index = 4; // Start after mesh attributes (0,1,2,3)

        for (const auto& element : layout.GetElements())
        {
            if (element.Type == ShaderDataType::Mat4)
            {
                std::size_t vec4Size = sizeof(glm::vec4);
                for (uint32_t i = 0; i < 4; i++)
                {
                    glEnableVertexAttribArray(index + i);
                    glVertexAttribPointer(index + i, 4, GL_FLOAT, GL_FALSE,
                                          layout.GetStride(), // Stride correto: tamanho total da estrutura
                                          (void*)(element.Offset + i * vec4Size)); // Offset correto: offset do elemento + deslocamento do vec4
                    glVertexAttribDivisor(index + i, 1);
                }
                index += 4;
            }
            else if (element.Type == ShaderDataType::Float4)
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE,
                                      layout.GetStride(), (void*)element.Offset); // Já estava correto
                glVertexAttribDivisor(index, 1);
                index++;
            }
        }

        vao->Unbind();
    }

    void InstancedRenderer::RenderInstanced(Ref<VertexArray> vao, Ref<Mesh> mesh, uint32_t instanceCount)
    {
        // Use default material if available
        if (m_DefaultMaterial && m_DefaultMaterial->GetAlbedoMap())
        {
            m_DefaultMaterial->GetAlbedoMap()->Bind(0);
            if (m_DefaultMaterial->GetNormalMap())
                m_DefaultMaterial->GetNormalMap()->Bind(1);
            if (m_DefaultMaterial->GetMetallicMap())
                m_DefaultMaterial->GetMetallicMap()->Bind(2);
            if (m_DefaultMaterial->GetRoughnessMap())
                m_DefaultMaterial->GetRoughnessMap()->Bind(3);
        }
        else
        {
            // Create a simple white texture for fallback
            static Ref<Texture2D> fallbackTexture = nullptr;
            if (!fallbackTexture)
            {
                fallbackTexture = Texture2D::Create(TextureSpecification());
                uint32_t whiteData = 0xffffffff;
                fallbackTexture->SetData(&whiteData, sizeof(uint32_t));
            }

            fallbackTexture->Bind(0);
            fallbackTexture->Bind(1);
            fallbackTexture->Bind(2);
            fallbackTexture->Bind(3);
        }

        m_InstancedShader->Bind();
        m_InstancedShader->SetInt("u_AlbedoMap", 0);
        m_InstancedShader->SetInt("u_NormalMap", 1);
        m_InstancedShader->SetInt("u_MetallicMap", 2);
        m_InstancedShader->SetInt("u_RoughnessMap", 3);

        vao->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(),
                                GL_UNSIGNED_INT, 0, instanceCount);
        vao->Unbind();
    }

    void InstancedRenderer::ClearCache()
    {
        m_InstancedVAOs.clear();
        m_Stats.CachedVAOs = 0;

#ifdef FENGINE_RENDER_DEBUG
        FENGINE_CORE_INFO("Instanced renderer cache cleared");
#endif

    }

    void InstancedRenderer::ResetStats()
    {
        m_Stats = InstancedStats{};
    }


    static InstancedRenderer s_GlobalInstancedRenderer;

    namespace InstancedRendering
    {
        void Initialize()
        {
            s_GlobalInstancedRenderer.Init();
        }

        void Shutdown()
        {
            s_GlobalInstancedRenderer.Shutdown();
        }

        void DrawMesh(const std::vector<glm::mat4>& transforms,
                      Ref<Mesh> mesh,
                      const std::vector<glm::vec4>& colors,
                      const std::vector<int>& entityIDs)
        {
            s_GlobalInstancedRenderer.DrawInstancedMesh(transforms, mesh, colors, entityIDs);
        }

        InstancedRenderer::InstancedStats GetStats()
        {
            return s_GlobalInstancedRenderer.GetStats();
        }

        void ResetStats()
        {
            s_GlobalInstancedRenderer.ResetStats();
        }
    }
} // namespace ForgeEngine
