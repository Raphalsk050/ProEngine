#include "Platform/Filament/FilamentEngine.h"
#include <utils/EntityManager.h>
#include <filament/Viewport.h>

namespace ProEngine {

FilamentEngine::~FilamentEngine() {
    Destroy();
}

void FilamentEngine::Initialize(uint32_t width, uint32_t height) {
    engine_ = filament::Engine::Builder()
        .backend(filament::Engine::Backend::OPENGL)
        .build();
    renderer_ = engine_->createRenderer();
    swap_chain_ = engine_->createSwapChain(nullptr);
    scene_ = engine_->createScene();
    view_ = engine_->createView();

    camera_entity_ = utils::EntityManager::get().create();
    camera_ = engine_->createCamera(camera_entity_);
    camera_->setProjection(45.0f, width / static_cast<float>(height), 0.1f, 1000.0f);

    view_->setScene(scene_);
    view_->setCamera(camera_);
    filament::Viewport vp{0, 0, width, height};
    view_->setViewport(vp);
    view_->setPostProcessingEnabled(false);
}

void FilamentEngine::Destroy() {
    if (!engine_) return;
    if (view_) engine_->destroy(view_);
    if (scene_) engine_->destroy(scene_);
    if (renderer_) engine_->destroy(renderer_);
    if (swap_chain_) engine_->destroy(swap_chain_);
    engine_->destroyCameraComponent(camera_entity_);
    filament::Engine::destroy(&engine_);
    engine_ = nullptr;
    renderer_ = nullptr;
    swap_chain_ = nullptr;
    scene_ = nullptr;
    view_ = nullptr;
    camera_ = nullptr;
}

} // namespace ProEngine
