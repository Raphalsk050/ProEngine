#pragma once

#include "Core/Layer/Layer.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Window/Window.h"
#include "imgui.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/Skybox.h>
#include <filament/RenderTarget.h>
#include <filament/Texture.h>

#include <gltfio/AssetLoader.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/FilamentAsset.h>

#include <utils/Entity.h>

#include "Core/Camera/FilamentCamera.h"
#include "Core/Camera/FilamentCameraController.h"

namespace ProEngine {

class SceneLayer : public Layer {
public:
    SceneLayer();
    virtual ~SceneLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& e) override;

private:
    // Filament objects
    filament::Engine* engine_ = nullptr;
    filament::Renderer* renderer_ = nullptr;
    filament::SwapChain* swap_chain_ = nullptr;
    filament::Scene* scene_ = nullptr;
    filament::View* view_ = nullptr;
    filament::Camera* camera_ = nullptr;
    filament::Skybox* skybox_ = nullptr;
    filament::RenderTarget* render_target_ = nullptr;
    filament::Texture* color_texture_ = nullptr;

    utils::Entity camera_entity_;

    // GLTF loading
    filament::gltfio::AssetLoader* asset_loader_ = nullptr;
    filament::gltfio::ResourceLoader* resource_loader_ = nullptr;
    filament::gltfio::FilamentAsset* asset_ = nullptr;

    void CopyFilamentToOpenGL();

    // Render target dimensions
    int render_width_ = 800;
    int render_height_ = 600;
    float camera_fov_ = 45.0f;
    float camera_distance_ = 10.0f;
    float camera_angle_ = 0.0f;
    float camera_height_ = 0.0f;

    // OpenGL objects for ImGui integration
    uint opengl_texture_id_ = 0;
    uint framebuffer_gl_id_ = 0;
    uint depth_buffer_id_ = 0;

    // Control flags
    bool show_test_pattern_ = false;
    bool model_loaded_ = false;

    uint8_t* pixel_buffer_ = nullptr;
    glm::vec3 model_center_ = {0.0f, 0.0f, 0.0f};

    // Your existing framebuffer
    Ref<Framebuffer> framebuffer_;
    glm::vec2 window_size_;

    // camera
    FilamentCameraController camera_controller_ = FilamentCameraController();

private:
    void CreateOpenGLFramebuffer();
    void CreateTestGeometry();
    void LoadGLTFModel();
    void CreateFallbackCube();
    void SetupLighting();
    void FillTestPattern();
    void UpdateCamera(float distance, float angle, float height);
};

} // namespace ProEngine