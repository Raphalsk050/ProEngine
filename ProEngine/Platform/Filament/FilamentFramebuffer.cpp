#include "Platform/Filament/FilamentFramebuffer.h"
#include "PEPCH.h"
#include <glad/glad.h>
#include <utils/EntityManager.h>
#include <gltfio/AssetLoader.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/MaterialProvider.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/TextureProvider.h>
#include <algorithm>
#include <fstream>
#include <vector>

#include "filament/LightManager.h"
#include "filament/Viewport.h"

namespace ProEngine {

FilamentFramebuffer::FilamentFramebuffer(const FramebufferSpecification& spec) {
    specification_ = spec;
    CreateResources();
}

FilamentFramebuffer::~FilamentFramebuffer() {
    DestroyResources();
}

void FilamentFramebuffer::CreateResources() {
    render_width_ = specification_.Width;
    render_height_ = specification_.Height;

    engine_ = filament::Engine::Builder()
                  .backend(filament::Engine::Backend::OPENGL)
                  .build();
    renderer_ = engine_->createRenderer();
    swap_chain_ = engine_->createSwapChain(1, 1);

    color_texture_ = filament::Texture::Builder()
                         .width(render_width_)
                         .height(render_height_)
                         .levels(1)
                         .usage(filament::Texture::Usage::COLOR_ATTACHMENT |
                                filament::Texture::Usage::SAMPLEABLE)
                         .format(filament::Texture::InternalFormat::RGBA8)
                         .build(*engine_);

    depth_texture_ = filament::Texture::Builder()
                         .width(render_width_)
                         .height(render_height_)
                         .levels(1)
                         .usage(filament::Texture::Usage::DEPTH_ATTACHMENT)
                         .format(filament::Texture::InternalFormat::DEPTH24)
                         .build(*engine_);

    render_target_ = filament::RenderTarget::Builder()
                         .texture(filament::RenderTarget::AttachmentPoint::COLOR,
                                  color_texture_)
                         .texture(filament::RenderTarget::AttachmentPoint::DEPTH,
                                  depth_texture_)
                         .build(*engine_);

    scene_ = engine_->createScene();
    view_ = engine_->createView();

    camera_entity_ = utils::EntityManager::get().create();
    camera_ = engine_->createCamera(camera_entity_);
    camera_->setProjection(45.0f,
                           render_width_ / static_cast<float>(render_height_),
                           0.1f, 1000.0f);
    camera_->lookAt({0, 0, 5}, {0, 0, 0});

    view_->setScene(scene_);
    view_->setCamera(camera_);
    filament::Viewport vp{0, 0, (uint32_t)render_width_, (uint32_t)render_height_};
    view_->setViewport(vp);
    view_->setRenderTarget(render_target_);
    view_->setPostProcessingEnabled(false);

    pixel_buffer_ = new uint8_t[render_width_ * render_height_ * 4];

    glGenTextures(1, &opengl_texture_id_);
    glBindTexture(GL_TEXTURE_2D, opengl_texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, render_width_, render_height_, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &framebuffer_gl_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_gl_id_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           opengl_texture_id_, 0);

    glGenRenderbuffers(1, &depth_buffer_id_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, render_width_,
                          render_height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                              depth_buffer_id_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    LoadModel();
    SetupLighting();
}

void FilamentFramebuffer::DestroyResources() {
    if (engine_) {
        if (render_target_) engine_->destroy(render_target_);
        if (color_texture_) engine_->destroy(color_texture_);
        if (depth_texture_) engine_->destroy(depth_texture_);
        if (view_) engine_->destroy(view_);
        if (scene_) engine_->destroy(scene_);
        if (renderer_) engine_->destroy(renderer_);
        if (swap_chain_) engine_->destroy(swap_chain_);
        engine_->destroyCameraComponent(camera_entity_);
        filament::Engine::destroy(&engine_);
    }

    if (opengl_texture_id_) glDeleteTextures(1, &opengl_texture_id_);
    if (framebuffer_gl_id_) glDeleteFramebuffers(1, &framebuffer_gl_id_);
    if (depth_buffer_id_) glDeleteRenderbuffers(1, &depth_buffer_id_);
    delete[] pixel_buffer_;
}

void FilamentFramebuffer::CopyFilamentToOpenGL() const {
    filament::backend::PixelBufferDescriptor buffer(
        pixel_buffer_,
        render_width_ * render_height_ * 4,
        filament::backend::PixelDataFormat::RGBA,
        filament::backend::PixelDataType::UBYTE);

    renderer_->readPixels(render_target_, 0, 0, render_width_, render_height_,
                          std::move(buffer));
    engine_->flushAndWait();

    glBindTexture(GL_TEXTURE_2D, opengl_texture_id_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, render_width_, render_height_,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer_);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FilamentFramebuffer::Bind() const {
    if (renderer_->beginFrame(swap_chain_)) {
        renderer_->render(view_);
        renderer_->endFrame();
    }
    CopyFilamentToOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_gl_id_);
}

void FilamentFramebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FilamentFramebuffer::Resize(uint32_t width, uint32_t height) {
    // simple implementation: recreate everything
    specification_.Width = width;
    specification_.Height = height;
    DestroyResources();
    CreateResources();
}

void FilamentFramebuffer::LoadModel() {
    auto* matProvider = filament::gltfio::createJitShaderProvider(engine_);
    asset_loader_ = filament::gltfio::AssetLoader::create({engine_, matProvider});
    resource_loader_ = new filament::gltfio::ResourceLoader({engine_, "../ProEngine/Assets/Models", true});
    auto stb_decoder = filament::gltfio::createStbProvider(engine_);
    resource_loader_->addTextureProvider("image/png", stb_decoder);
    resource_loader_->addTextureProvider("image/jpeg", stb_decoder);

    const char* path = "../ProEngine/Assets/Models/DragonAttenuation.glb";
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return;
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    asset_ = asset_loader_->createAsset(buffer.data(), (uint32_t)buffer.size());
    if (!asset_) return;
    resource_loader_->loadResources(asset_);
    asset_->releaseSourceData();
    scene_->addEntities(asset_->getEntities(), asset_->getEntityCount());

    auto aabb = asset_->getBoundingBox();
    glm::vec3 max = {aabb.max.x, aabb.max.y, aabb.max.z};
    glm::vec3 min = {aabb.min.x, aabb.min.y, aabb.min.z};
    model_center_ = (max + min) * 0.5f;
    float sizeMax = std::max({max.x - min.x, max.y - min.y, max.z - min.z});
    float distance = sizeMax * 3.0f;
    camera_->lookAt({model_center_.x, model_center_.y, model_center_.z + distance},
                    {model_center_.x, model_center_.y, model_center_.z},
                    {0,1,0});
}

void FilamentFramebuffer::SetupLighting() {
    auto light = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::SUN)
        .color({1.0f,1.0f,1.0f})
        .intensity(500000.0f)
        .direction({0,-1,0})
        .castShadows(false)
        .build(*engine_, light);
    scene_->addEntity(light);
}

} // namespace ProEngine
