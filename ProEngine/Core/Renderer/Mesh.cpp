#include "Core/Renderer/Mesh.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "VertexArray.h"

namespace ProEngine {

Mesh::Mesh() {
  m_VertexArray = VertexArray::Create();
  if (!m_VertexArray) {
    PENGINE_CORE_ASSERT(false, "Vertex Array Creation Failed");
    PENGINE_CORE_CRITICAL("Vertex Array Creation Failed");
  }
}

  void Mesh::SetVertices(std::vector<float>& vertices, uint32_t vertexCount) {
  m_VertexCount = vertexCount;

  m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));

  // ✅ Adicionar layout padrão ou exigir que seja definido externamente
  if (!m_VertexBuffer->GetLayout().GetElements().empty()) {
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
  } else {
    PENGINE_CORE_ASSERT(false, "Layout is not defined!")
  }
}

void Mesh::SetIndices(std::vector<uint32_t>& indices) {
  m_IndexCount = indices.size();

  m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
  m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void Mesh::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
  m_VertexBuffer = vertexBuffer;
  m_VertexArray->AddVertexBuffer(vertexBuffer);
}

void Mesh::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
  m_IndexBuffer = indexBuffer;
  m_IndexCount = indexBuffer->GetCount();
  m_VertexArray->SetIndexBuffer(indexBuffer);
}

Ref<Mesh> Mesh::CreateCube(float size) {
  Ref<Mesh> mesh = CreateRef<Mesh>();

  // Define vertex layout
  BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" },
    { ShaderDataType::Float3, "a_Tangent" },
    { ShaderDataType::Float2, "a_TexCoord" }
  };

  // Create cube vertices
  // This is a simplified cube with positions, normals, tangents, and texture coordinates
  float halfSize = size * 0.5f;
  std::vector<float> vertices = {
    // Front face - each vertex has position (3), normal (3), tangent (3), texcoord (2)
    -halfSize, -halfSize,  halfSize,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left
     halfSize, -halfSize,  halfSize,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom-right
     halfSize,  halfSize,  halfSize,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-right
    -halfSize,  halfSize,  halfSize,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top-left

    // Back face
    -halfSize, -halfSize, -halfSize,  0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom-left
    -halfSize,  halfSize, -halfSize,  0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-left
     halfSize,  halfSize, -halfSize,  0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top-right
     halfSize, -halfSize, -halfSize,  0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-right

    // Left face
    -halfSize, -halfSize, -halfSize, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
    -halfSize, -halfSize,  halfSize, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, // bottom-right
    -halfSize,  halfSize,  halfSize, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
    -halfSize,  halfSize, -halfSize, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, // top-left

    // Right face
     halfSize, -halfSize,  halfSize,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left
     halfSize, -halfSize, -halfSize,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom-right
     halfSize,  halfSize, -halfSize,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
     halfSize,  halfSize,  halfSize,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // top-left

    // Bottom face
    -halfSize, -halfSize, -halfSize,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left
     halfSize, -halfSize, -halfSize,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom-right
     halfSize, -halfSize,  halfSize,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-right
    -halfSize, -halfSize,  halfSize,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top-left

    // Top face
    -halfSize,  halfSize,  halfSize,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom-left
     halfSize,  halfSize,  halfSize,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom-right
     halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top-right
    -halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top-left
  };

  // Create indices for the cube
  std::vector<uint32_t> indices = {
    // Front face
    0, 1, 2, 2, 3, 0,

    // Back face
    4, 5, 6, 6, 7, 4,

    // Left face
    8, 9, 10, 10, 11, 8,

    // Right face
    12, 13, 14, 14, 15, 12,

    // Bottom face
    16, 17, 18, 18, 19, 16,

    // Top face
    20, 21, 22, 22, 23, 20
  };

  // Set up the mesh
  Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
  vertexBuffer->SetLayout(layout);
  mesh->SetVertexBuffer(vertexBuffer);
  mesh->m_VertexCount = 24; // 6 faces * 4 vertices per face

  mesh->SetIndices(indices);

  return mesh;
}

Ref<Mesh> Mesh::CreateSphere(float radius, uint32_t segmentsX, uint32_t segmentsY) {
  Ref<Mesh> mesh = CreateRef<Mesh>();

  // Define vertex layout
  BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" },
    { ShaderDataType::Float3, "a_Tangent" },
    { ShaderDataType::Float2, "a_TexCoord" }
  };

  std::vector<float> vertices;
  std::vector<uint32_t> indices;

  const float PI = static_cast<float>(M_PI);

  // Calculate vertices
  for (uint32_t y = 0; y <= segmentsY; ++y) {
    for (uint32_t x = 0; x <= segmentsX; ++x) {
      // Calculate position
      float xSegment = (float)x / (float)segmentsX;
      float ySegment = (float)y / (float)segmentsY;
      float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
      float yPos = std::cos(ySegment * PI);
      float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

      // Calculate normal (same as position for a sphere)
      float nx = xPos;
      float ny = yPos;
      float nz = zPos;

      // Calculate tangent
      float tx = -std::sin(xSegment * 2.0f * PI);
      float ty = 0.0f;
      float tz = std::cos(xSegment * 2.0f * PI);

      // Normalize tangent
      float length = std::sqrt(tx * tx + ty * ty + tz * tz);
      if (length > 0.0f) {
        tx /= length;
        ty /= length;
        tz /= length;
      }

      // Add to vertices
      vertices.push_back(xPos * radius);    // Position X
      vertices.push_back(yPos * radius);    // Position Y
      vertices.push_back(zPos * radius);    // Position Z
      vertices.push_back(nx);               // Normal X
      vertices.push_back(ny);               // Normal Y
      vertices.push_back(nz);               // Normal Z
      vertices.push_back(tx);               // Tangent X
      vertices.push_back(ty);               // Tangent Y
      vertices.push_back(tz);               // Tangent Z
      vertices.push_back(xSegment);         // Texture U
      vertices.push_back(ySegment);         // Texture V
    }
  }

  // Calculate indices
  for (uint32_t y = 0; y < segmentsY; ++y) {
    for (uint32_t x = 0; x < segmentsX; ++x) {
      uint32_t first = (y * (segmentsX + 1)) + x;
      uint32_t second = first + 1;
      uint32_t third = first + (segmentsX + 1);
      uint32_t fourth = third + 1;

      indices.push_back(first);
      indices.push_back(third);
      indices.push_back(second);

      indices.push_back(second);
      indices.push_back(third);
      indices.push_back(fourth);
    }
  }

  // Set up the mesh
  Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
  vertexBuffer->SetLayout(layout);
  mesh->SetVertexBuffer(vertexBuffer);
  mesh->m_VertexCount = (segmentsX + 1) * (segmentsY + 1);

  mesh->SetIndices(indices);

  return mesh;
}

Ref<Mesh> Mesh::CreateCylinder(float radius, float height, uint32_t segments) {
  Ref<Mesh> mesh = CreateRef<Mesh>();

  // Define vertex layout
  BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" },
    { ShaderDataType::Float3, "a_Tangent" },
    { ShaderDataType::Float2, "a_TexCoord" }
  };

  std::vector<float> vertices;
  std::vector<uint32_t> indices;

  const float PI = static_cast<float>(M_PI);
  float halfHeight = height * 0.5f;

  // Create top and bottom center vertices
  // Top center vertex (center top)
  vertices.push_back(0.0f);            // Position X
  vertices.push_back(halfHeight);      // Position Y
  vertices.push_back(0.0f);            // Position Z
  vertices.push_back(0.0f);            // Normal X
  vertices.push_back(1.0f);            // Normal Y
  vertices.push_back(0.0f);            // Normal Z
  vertices.push_back(1.0f);            // Tangent X
  vertices.push_back(0.0f);            // Tangent Y
  vertices.push_back(0.0f);            // Tangent Z
  vertices.push_back(0.5f);            // Texture U
  vertices.push_back(0.5f);            // Texture V

  // Bottom center vertex (center bottom)
  vertices.push_back(0.0f);            // Position X
  vertices.push_back(-halfHeight);     // Position Y
  vertices.push_back(0.0f);            // Position Z
  vertices.push_back(0.0f);            // Normal X
  vertices.push_back(-1.0f);           // Normal Y
  vertices.push_back(0.0f);            // Normal Z
  vertices.push_back(1.0f);            // Tangent X
  vertices.push_back(0.0f);            // Tangent Y
  vertices.push_back(0.0f);            // Tangent Z
  vertices.push_back(0.5f);            // Texture U
  vertices.push_back(0.5f);            // Texture V

  // Create vertices for the cylinder sides, top, and bottom
  for (uint32_t i = 0; i <= segments; i++) {
    float angle = (float)i / (float)segments * 2.0f * PI;
    float x = std::cos(angle);
    float z = std::sin(angle);

    // Top rim vertex
    vertices.push_back(x * radius);     // Position X
    vertices.push_back(halfHeight);     // Position Y
    vertices.push_back(z * radius);     // Position Z
    vertices.push_back(0.0f);           // Normal X
    vertices.push_back(1.0f);           // Normal Y
    vertices.push_back(0.0f);           // Normal Z
    vertices.push_back(1.0f);           // Tangent X
    vertices.push_back(0.0f);           // Tangent Y
    vertices.push_back(0.0f);           // Tangent Z
    vertices.push_back((x + 1.0f) * 0.5f); // Texture U
    vertices.push_back((z + 1.0f) * 0.5f); // Texture V

    // Bottom rim vertex
    vertices.push_back(x * radius);     // Position X
    vertices.push_back(-halfHeight);    // Position Y
    vertices.push_back(z * radius);     // Position Z
    vertices.push_back(0.0f);           // Normal X
    vertices.push_back(-1.0f);          // Normal Y
    vertices.push_back(0.0f);           // Normal Z
    vertices.push_back(1.0f);           // Tangent X
    vertices.push_back(0.0f);           // Tangent Y
    vertices.push_back(0.0f);           // Tangent Z
    vertices.push_back((x + 1.0f) * 0.5f); // Texture U
    vertices.push_back((z + 1.0f) * 0.5f); // Texture V

    // Side vertex top
    vertices.push_back(x * radius);     // Position X
    vertices.push_back(halfHeight);     // Position Y
    vertices.push_back(z * radius);     // Position Z
    vertices.push_back(x);              // Normal X
    vertices.push_back(0.0f);           // Normal Y
    vertices.push_back(z);              // Normal Z
    vertices.push_back(-z);             // Tangent X
    vertices.push_back(0.0f);           // Tangent Y
    vertices.push_back(x);              // Tangent Z
    vertices.push_back(float(i) / float(segments)); // Texture U
    vertices.push_back(1.0f);           // Texture V

    // Side vertex bottom
    vertices.push_back(x * radius);     // Position X
    vertices.push_back(-halfHeight);    // Position Y
    vertices.push_back(z * radius);     // Position Z
    vertices.push_back(x);              // Normal X
    vertices.push_back(0.0f);           // Normal Y
    vertices.push_back(z);              // Normal Z
    vertices.push_back(-z);             // Tangent X
    vertices.push_back(0.0f);           // Tangent Y
    vertices.push_back(x);              // Tangent Z
    vertices.push_back(float(i) / float(segments)); // Texture U
    vertices.push_back(0.0f);           // Texture V
  }

  // Create indices
  uint32_t topCenterIndex = 0;
  uint32_t bottomCenterIndex = 1;
  uint32_t topRimStartIndex = 2;
  uint32_t bottomRimStartIndex = 3;
  uint32_t sideTopStartIndex = 4;
  uint32_t sideBottomStartIndex = 5;

  // Top face indices
  for (uint32_t i = 0; i < segments; i++) {
    indices.push_back(topCenterIndex);
    indices.push_back(topRimStartIndex + i * 4);
    indices.push_back(topRimStartIndex + ((i + 1) % segments) * 4);
  }

  // Bottom face indices
  for (uint32_t i = 0; i < segments; i++) {
    indices.push_back(bottomCenterIndex);
    indices.push_back(bottomRimStartIndex + ((i + 1) % segments) * 4);
    indices.push_back(bottomRimStartIndex + i * 4);
  }

  // Side face indices
  for (uint32_t i = 0; i < segments; i++) {
    uint32_t nextI = (i + 1) % segments;

    indices.push_back(sideTopStartIndex + i * 4);
    indices.push_back(sideBottomStartIndex + i * 4);
    indices.push_back(sideBottomStartIndex + nextI * 4);

    indices.push_back(sideTopStartIndex + i * 4);
    indices.push_back(sideBottomStartIndex + nextI * 4);
    indices.push_back(sideTopStartIndex + nextI * 4);
  }

  // Set up the mesh
  Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
  vertexBuffer->SetLayout(layout);
  mesh->SetVertexBuffer(vertexBuffer);
  mesh->m_VertexCount = 2 + 4 * (segments + 1); // Centers + 4 vertices per segment

  mesh->SetIndices(indices);

  return mesh;
}

Ref<Mesh> Mesh::CreatePlane(float width, float height) {
  Ref<Mesh> mesh = CreateRef<Mesh>();

  // Define vertex layout
  BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float3, "a_Normal" },
    { ShaderDataType::Float3, "a_Tangent" },
    { ShaderDataType::Float2, "a_TexCoord" }
  };

  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;

  // Create plane vertices (facing up in Y direction)
  std::vector<float> vertices = {
    // Position (3), Normal (3), Tangent (3), TexCoord (2)
    -halfWidth, 0.0f, -halfHeight,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     halfWidth, 0.0f, -halfHeight,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     halfWidth, 0.0f,  halfHeight,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -halfWidth, 0.0f,  halfHeight,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
  };

  // Create plane indices
  std::vector<uint32_t> indices = {
    1, 0, 2,
    2, 0, 3
  };

  // Set up the mesh
  Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
  vertexBuffer->SetLayout(layout);
  mesh->SetVertexBuffer(vertexBuffer);
  mesh->m_VertexCount = 4;

  mesh->SetIndices(indices);

  return mesh;
}

// Model implementation

Ref<Model> Model::Load(const std::string& filepath) {
  // This is a placeholder - in a real engine, this would use a model loading library
  // like Assimp to load models from various formats (OBJ, FBX, etc.)

  Ref<Model> model = CreateRef<Model>();

  // For now, we'll just return a simple cube model as an example
  model->AddMesh(Mesh::CreateCube());

  return model;
}

} // namespace BEngine