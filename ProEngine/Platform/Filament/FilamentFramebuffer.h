#pragma once
#include "Core/Renderer/Framebuffer.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/RenderTarget.h>
#include <filament/Texture.h>
#include <utils/Entity.h>
#include <glm.hpp>
#include <backend/PixelBufferDescriptor.h>

#include "gltfio/AssetLoader.h"
#include "gltfio/ResourceLoader.h"

namespace ProEngine {

class FilamentFramebuffer : public Framebuffer {
public:
    explicit FilamentFramebuffer(const FramebufferSpecification& spec);
    ~FilamentFramebuffer() override;

    void Bind() const override;
    void Unbind() const override;
    void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetColorAttachmentRendererID() const override { return opengl_texture_id_; }
    uint32_t GetEntityIDAttachmentRendererID() const override { return 0; }
    uint32_t GetDepthAttachmentRendererID() const override { return depth_buffer_id_; }
    uint32_t GetRendererID() const override { return framebuffer_gl_id_; }

    filament::Engine* GetEngine() const { return engine_; }
    filament::Scene* GetScene() const { return scene_; }
    filament::View* GetView() const { return view_; }
    filament::Camera* GetCamera() const { return camera_; }

private:
    void CreateResources();
    void DestroyResources();
    void CopyFilamentToOpenGL() const;
    void LoadModel();
    void SetupLighting();

    mutable filament::Engine* engine_ = nullptr;
    filament::Renderer* renderer_ = nullptr;
    filament::SwapChain* swap_chain_ = nullptr;
    filament::Scene* scene_ = nullptr;
    filament::View* view_ = nullptr;
    filament::Camera* camera_ = nullptr;
    filament::RenderTarget* render_target_ = nullptr;
    filament::Texture* color_texture_ = nullptr;
    filament::Texture* depth_texture_ = nullptr;
    filament::gltfio::AssetLoader* asset_loader_ = nullptr;
    filament::gltfio::ResourceLoader* resource_loader_ = nullptr;
    filament::gltfio::FilamentAsset* asset_ = nullptr;

    int render_width_ = 0;
    int render_height_ = 0;

    mutable uint8_t* pixel_buffer_ = nullptr;

    GLuint opengl_texture_id_ = 0;
    GLuint framebuffer_gl_id_ = 0;
    GLuint depth_buffer_id_ = 0;
    utils::Entity camera_entity_;

    glm::vec3 model_center_{0.0f};
};

} // namespace ProEngine
