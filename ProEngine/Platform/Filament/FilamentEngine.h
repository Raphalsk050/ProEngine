#pragma once
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <utils/Entity.h>

namespace ProEngine {

class FilamentEngine {
public:
    FilamentEngine() = default;
    ~FilamentEngine();

    void Initialize(uint32_t width, uint32_t height);
    void Destroy();

    filament::Engine* GetEngine() const { return engine_; }
    filament::Renderer* GetRenderer() const { return renderer_; }
    filament::Scene* GetScene() const { return scene_; }
    filament::View* GetView() const { return view_; }
    filament::Camera* GetCamera() const { return camera_; }

private:
    filament::Engine* engine_ = nullptr;
    filament::Renderer* renderer_ = nullptr;
    filament::SwapChain* swap_chain_ = nullptr;
    filament::Scene* scene_ = nullptr;
    filament::View* view_ = nullptr;
    filament::Camera* camera_ = nullptr;
    utils::Entity camera_entity_{};
};

} // namespace ProEngine
