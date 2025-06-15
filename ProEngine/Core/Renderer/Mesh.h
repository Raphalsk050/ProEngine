#pragma once

#include "Config.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Material.h"

namespace ProEngine
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh() = default;

        void SetVertices(std::vector<float>& vertices, uint32_t vertexCount);
        void SetIndices(std::vector<uint32_t>& indices);

        void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }
        uint32_t GetVertexCount() const { return m_VertexCount; }
        uint32_t GetIndexCount() const { return m_IndexCount; }

        void SetMaterial(const Ref<Material>& material) { m_Material = material; }
        Ref<Material> GetMaterial() const { return m_Material; }

        // Static factory methods for primitive creation
        static Ref<Mesh> CreateCube(float size = 1.0f);
        static Ref<Mesh> CreateSphere(float radius = 0.5f, uint32_t segmentsX = 8, uint32_t segmentsY = 8);
        static Ref<Mesh> CreateCylinder(float radius = 0.5f, float height = 1.0f, uint32_t segments = 16);
        static Ref<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f);

    private:
        friend class TestMesh;
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Material> m_Material;

        uint32_t m_VertexCount = 0;
        uint32_t m_IndexCount = 0;
    };

    // A model class to hold multiple meshes
    class Model
    {
    public:
        Model() = default;
        ~Model() = default;

        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
        void AddMesh(const Ref<Mesh>& mesh) { m_Meshes.push_back(mesh); }

        // Factory method for loading models from files
        static Ref<Model> Load(const std::string& filepath);

    private:
        std::vector<Ref<Mesh>> m_Meshes;
    };
} // namespace BEngine
