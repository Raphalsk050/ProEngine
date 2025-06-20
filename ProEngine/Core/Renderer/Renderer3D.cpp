#include "Core/Renderer/Renderer3D.h"

namespace ProEngine {

FilamentEngine Renderer3D::s_Engine;

bool PerformCulling(int, const glm::mat4&, float*) { return true; }

void Renderer3D::Init(uint32_t width, uint32_t height) {
    s_Engine.Initialize(width, height);
}

void Renderer3D::Shutdown() {
    s_Engine.Destroy();
}

void Renderer3D::BeginScene(const FilamentCamera& camera) {
    auto* filamentCamera = s_Engine.GetCamera();
    if (filamentCamera) {
        filamentCamera->lookAt({camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z},
                               {camera.GetPosition().x + camera.GetForwardDirection().x,
                                camera.GetPosition().y + camera.GetForwardDirection().y,
                                camera.GetPosition().z + camera.GetForwardDirection().z});
    }
}

void Renderer3D::EndScene() {
    auto* renderer = s_Engine.GetRenderer();
    auto* swap = s_Engine.GetEngine()->createSwapChain(nullptr);
    if (renderer->beginFrame(swap)) {
        renderer->render(s_Engine.GetView());
        renderer->endFrame();
    }
    s_Engine.GetEngine()->destroy(swap);
}

void Renderer3D::DrawModel(const glm::mat4& transform, ModelRendererComponent& src, int entityID) {
    (void)transform; (void)src; (void)entityID;
    // placeholder for model rendering with filament
}

} // namespace ProEngine
