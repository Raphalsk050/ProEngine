#pragma once
#include "Platform/Filament/FilamentEngine.h"
#include "Core/Camera/FilamentCamera.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Material.h"
#include "Core/Scene/Components.h"

namespace ProEngine {

class Renderer3D {
public:
    static void Init(uint32_t width = 1280, uint32_t height = 720);
    static void Shutdown();

    static void BeginScene(const FilamentCamera& camera);
    static void EndScene();

    static void DrawModel(const glm::mat4& transform, ModelRendererComponent& src, int entityID = -1);
    static void DrawMesh(const glm::mat4&, Ref<Mesh>, const glm::vec4&, int = -1) {}
    static void DrawMesh(const glm::mat4&, Ref<Mesh>, Ref<Material>, int = -1) {}
    static void DrawCube(const glm::mat4&, const glm::vec4&, int = -1) {}
    static void DrawSphere(const glm::mat4&, const glm::vec4&, int = -1) {}
    static void DrawBox(const glm::mat4&, const glm::vec4&, int = -1) {}

    static filament::Engine* GetEngine() { return s_Engine.GetEngine(); }

private:
    static FilamentEngine s_Engine;
};

} // namespace ProEngine
