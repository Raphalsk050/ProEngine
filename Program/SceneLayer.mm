#import <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA

#include "SceneLayer.h"
#include "imgui.h"
#include "Core/Application/Application.h"

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/LightManager.h>
#include <filament/Skybox.h>
#include <filament/Viewport.h>
#include <filament/Texture.h>
#include <filament/RenderTarget.h>
#include <filament/RenderableManager.h>
#include <backend/PixelBufferDescriptor.h>

#include <gltfio/AssetLoader.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/MaterialProvider.h>
#include <gltfio/FilamentAsset.h>
#include <gltfio/TextureProvider.h>

#include <utils/Entity.h>
#include <utils/EntityManager.h>

#include <fstream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace ProEngine {

SceneLayer::SceneLayer() : Layer("SceneLayer") {}

void SceneLayer::OnAttach() {
    Layer::OnAttach();

    FramebufferSpecification spec;
    spec.Width  = Application::Get().GetWindow().GetWidth();
    spec.Height = Application::Get().GetWindow().GetHeight();
    framebuffer_ = Framebuffer::Create(spec);
    window_size_ = {(float)spec.Width, (float)spec.Height};

    render_width_ = 800;
    render_height_ = 600;

    // Create Filament engine
    engine_ = filament::Engine::Builder()
                  .backend(filament::Engine::Backend::OPENGL)
                  .build();
    renderer_ = engine_->createRenderer();

    // Criar SwapChain dummy (necessário para beginFrame/endFrame)
    swap_chain_ = engine_->createSwapChain(1, 1);

    CreateOpenGLFramebuffer();

    // Criar texturas para render target do Filament
    color_texture_ = filament::Texture::Builder()
        .width(render_width_)
        .height(render_height_)
        .levels(1)
        .usage(filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE)
        .format(filament::Texture::InternalFormat::RGBA8)
        .build(*engine_);

    filament::Texture* depthTexture = filament::Texture::Builder()
        .width(render_width_)
        .height(render_height_)
        .levels(1)
        .usage(filament::Texture::Usage::DEPTH_ATTACHMENT)
        .format(filament::Texture::InternalFormat::DEPTH24)
        .build(*engine_);

    render_target_ = filament::RenderTarget::Builder()
        .texture(filament::RenderTarget::AttachmentPoint::COLOR, color_texture_)
        .texture(filament::RenderTarget::AttachmentPoint::DEPTH, depthTexture)
        .build(*engine_);

    // Scene setup
    scene_ = engine_->createScene();
    view_ = engine_->createView();

    camera_entity_ = utils::EntityManager::get().create();
    camera_ = engine_->createCamera(camera_entity_);
    camera_->setProjection(45.0f, render_width_ / (float)render_height_, 0.1f, 1000.0f);
    camera_->lookAt({0,0,5}, {0,0,0}, {0,1,0});

    view_->setScene(scene_);
    view_->setCamera(camera_);

    filament::Viewport vp{0, 0, static_cast<uint32_t>(render_width_), static_cast<uint32_t>(render_height_)};
    view_->setViewport(vp);
    view_->setRenderTarget(render_target_);
    view_->setPostProcessingEnabled(false);

    // Alocar buffer para cópia de pixels
    pixel_buffer_ = new uint8_t[render_width_ * render_height_ * 4];

    // PRIMEIRO: Criar uma geometria simples para testar
    CreateTestGeometry();

    // DEPOIS: Tentar carregar o modelo GLTF
    LoadGLTFModel();

    // Configurar iluminação MUITO forte
    SetupLighting();

    printf("=== SCENE SETUP COMPLETE ===\n");
}

void SceneLayer::CreateTestGeometry() {
    printf("=== CREATING TEST GEOMETRY ===\n");
    // Por enquanto, vamos focar no modelo GLTF
    printf("Test geometry creation skipped for now\n");
}

void SceneLayer::LoadGLTFModel() {
    printf("=== LOADING GLTF MODEL ===\n");

    auto* matProvider = filament::gltfio::createJitShaderProvider(engine_);
    asset_loader_ = filament::gltfio::AssetLoader::create({engine_, matProvider});
    resource_loader_ = new filament::gltfio::ResourceLoader({engine_, "../ProEngine/Assets/Models", true});
    auto stb_decoder = filament::gltfio::createStbProvider(engine_);
    resource_loader_->addTextureProvider("image/png", stb_decoder);
    resource_loader_->addTextureProvider("image/jpeg", stb_decoder);

    // Tentar múltiplos caminhos para o modelo
    std::vector<std::string> modelPaths = {
        "../ProEngine/Assets/Models/DragonAttenuation.glb",
    };

    std::vector<uint8_t> buffer;
    std::string loadedPath;

    for (const auto& path : modelPaths) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (file) {
            auto size = file.tellg();
            file.seekg(0, std::ios::beg);
            buffer.resize(size);
            file.read(reinterpret_cast<char*>(buffer.data()), size);
            file.close();
            loadedPath = path;
            printf("✓ Loaded model from: %s (%zu bytes)\n", path.c_str(), buffer.size());
            break;
        } else {
            printf("✗ Failed to load from: %s\n", path.c_str());
        }
    }

    if (buffer.empty()) {
        printf("✗ Could not load model from any path!\n");
        CreateFallbackCube();
        return;
    }

    asset_ = asset_loader_->createAsset(buffer.data(), (uint32_t)buffer.size());
    if (!asset_) {
        printf("✗ Failed to create asset from buffer\n");
        CreateFallbackCube();
        return;
    }

    printf("✓ Asset created successfully\n");
    printf("  Entity count: %zu\n", asset_->getEntityCount());

    // Carregar recursos de forma simples
    bool resourcesLoaded = resource_loader_->asyncBeginLoad(asset_);
    PENGINE_CORE_INFO("  Resources initiated: {}", resourcesLoaded ? "Yes" : "No");

    asset_->releaseSourceData();

    // Debug detalhado do bounding box
    auto aabb = asset_->getBoundingBox();

    printf("  Bounding box:\n");
    printf("    Min: (%.6f, %.6f, %.6f)\n", aabb.min.x, aabb.min.y, aabb.min.z);
    printf("    Max: (%.6f, %.6f, %.6f)\n", aabb.max.x, aabb.max.y, aabb.max.z);

    auto aabb_max = glm::vec3(aabb.max.x, aabb.max.y, aabb.max.z);
    auto aabb_min = glm::vec3(aabb.min.x, aabb.min.y, aabb.min.z);

    glm::vec3 center = (aabb_max + aabb_min) * 0.5f;
    glm::vec3 size = aabb_max - aabb_min;
    float maxSize = std::max({size.x, size.y, size.z});

    printf("    Center: (%.6f, %.6f, %.6f)\n", center.x, center.y, center.z);
    printf("    Size: (%.6f, %.6f, %.6f)\n", size.x, size.y, size.z);
    printf("    Max dimension: %.6f\n", maxSize);

    // Adicionar entidades à cena
    auto entities = asset_->getEntities();
    scene_->addEntities(entities, asset_->getEntityCount());
    printf("✓ Added %zu entities to scene\n", asset_->getEntityCount());

    // Debug de componentes renderizáveis
    auto& rcm = engine_->getRenderableManager();

    // Ajustar câmera para enquadrar o modelo perfeitamente
    if (maxSize > 0.001f) {
        // Posicionar câmera para ver o modelo
        float distance = maxSize * 3.0f;  // Distância baseada no tamanho
        camera_distance_ = distance;
        model_center_ = center;

        // Olhar para o centro do modelo
        camera_->lookAt(
            {center.x, center.y, center.z + distance}, // Posição da câmera
            {center.x, center.y, center.z},            // Olhar para o centro
            {0, 1, 0}                                   // Up vector
        );

        printf("  Camera positioned at distance %.6f from center\n", distance);
        printf("  Camera looking at center: (%.6f, %.6f, %.6f)\n", center.x, center.y, center.z);
    }
}

void SceneLayer::CreateFallbackCube() {
    printf("=== CREATING FALLBACK CUBE ===\n");
    printf("Fallback cube creation not implemented yet\n");
}

void SceneLayer::SetupLighting() {
    printf("=== SETTING UP LIGHTING ===\n");

    // Luz ambiente muito forte de várias direções
    auto light1 = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::DIRECTIONAL)
        .color({1.0f, 1.0f, 1.0f})
        .direction({-1, -1, -1})
        .intensity(200000.0f)  // Muito forte
        .castShadows(false)
        .build(*engine_, light1);
    scene_->addEntity(light1);

    auto light2 = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::DIRECTIONAL)
        .color({1.0f, 1.0f, 1.0f})
        .direction({1, -1, -1})
        .intensity(200000.0f)
        .castShadows(false)
        .build(*engine_, light2);
    scene_->addEntity(light2);

    auto light3 = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::DIRECTIONAL)
        .color({1.0f, 1.0f, 1.0f})
        .direction({0, 1, 0})   // De baixo para cima
        .intensity(100000.0f)
        .castShadows(false)
        .build(*engine_, light3);
    scene_->addEntity(light3);

    // Adicionar luz ambiente global
    auto light4 = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::SUN)
        .color({1.0f, 1.0f, 1.0f})
        .intensity(50000.0f)
        .direction({0, -1, 0})
        .castShadows(false)
        .build(*engine_, light4);
    scene_->addEntity(light4);

    printf("✓ Added 4 powerful lights (3 directional + 1 sun)\n");
}

void SceneLayer::CreateOpenGLFramebuffer() {
    glGenTextures(1, &opengl_texture_id_);
    glBindTexture(GL_TEXTURE_2D, opengl_texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, render_width_, render_height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &framebuffer_gl_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_gl_id_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opengl_texture_id_, 0);

    glGenRenderbuffers(1, &depth_buffer_id_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, render_width_, render_height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id_);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("✗ Framebuffer error: %d\n", status);
    } else {
        printf("✓ Framebuffer created successfully\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneLayer::CopyFilamentToOpenGL() {
    // Criar descriptor para ler pixels
    filament::backend::PixelBufferDescriptor buffer(
        pixel_buffer_,
        render_width_ * render_height_ * 4,
        filament::backend::PixelDataFormat::RGBA,
        filament::backend::PixelDataType::UBYTE
    );

    // Ler pixels do RenderTarget
    renderer_->readPixels(
        render_target_,
        0, 0, render_width_, render_height_,
        std::move(buffer)
    );

    // Aguardar a leitura completar
    engine_->flushAndWait();

    // Atualizar a textura OpenGL
    glBindTexture(GL_TEXTURE_2D, opengl_texture_id_);
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0,
        render_width_, render_height_,
        GL_RGBA, GL_UNSIGNED_BYTE,
        pixel_buffer_
    );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneLayer::OnUpdate(Timestep ts) {
    Layer::OnUpdate(ts);

    if (show_test_pattern_) {
        return;
    }

    // Atualizar recursos se necessário
    if (asset_ && resource_loader_) {
        resource_loader_->asyncUpdateLoad();
        float progress = resource_loader_->asyncGetLoadProgress();
        if (progress >= 1.0f && !model_loaded_) {
            model_loaded_ = true;
            printf("✓ Model finished loading!\n");
        }
    }

    // Salvar estado atual do OpenGL
    GLint currentFramebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Renderizar com Filament para o render target
    if (renderer_->beginFrame(swap_chain_)) {
        // Definir cor de fundo diferente para debug
        if (model_loaded_) {
            renderer_->setClearOptions({
                .clearColor = {0.1f, 0.1f, 0.2f, 1.0f},
                .clear = true
            });
        } else {
            renderer_->setClearOptions({
                .clearColor = {0.2f, 0.1f, 0.1f, 1.0f},
                .clear = true
            });
        }

        renderer_->render(view_);
        renderer_->endFrame();
    }

    // Copiar resultado do Filament para textura OpenGL
    CopyFilamentToOpenGL();

    // Restaurar estado OpenGL
    glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void SceneLayer::OnImGuiRender() {
    Layer::OnImGuiRender();

    ImGui::Begin("Filament Debug - Model Loading");

    // Status detalhado
    ImGui::Text("=== MODEL STATUS ===");
    ImGui::Text("Model loaded: %s", model_loaded_ ? "YES" : "NO");
    ImGui::Text("Asset pointer: %p", (void*)asset_);

    if (asset_) {
        ImGui::Text("Entity count: %zu", asset_->getEntityCount());
        auto aabb = asset_->getBoundingBox();
        ImGui::Text("Bounding box:");
        ImGui::Text("  Min: (%.3f, %.3f, %.3f)", aabb.min.x, aabb.min.y, aabb.min.z);
        ImGui::Text("  Max: (%.3f, %.3f, %.3f)", aabb.max.x, aabb.max.y, aabb.max.z);

        auto aabb_max = glm::vec3(aabb.max.x, aabb.max.y, aabb.max.z);
        auto aabb_min = glm::vec3(aabb.min.x, aabb.min.y, aabb.min.z);

        glm::vec3 center = (aabb_max + aabb_min) * 0.5f;
        glm::vec3 size = aabb_max - aabb_min;
        float maxSize = std::max({size.x, size.y, size.z});

        ImGui::Text("  Center: (%.3f, %.3f, %.3f)", center.x, center.y, center.z);
        ImGui::Text("  Size: %.3f", maxSize);

        if (asset_ && resource_loader_) {
            float progress = resource_loader_->asyncGetLoadProgress();
            ImGui::Text("Load progress: %.2f", progress);
            ImGui::ProgressBar(progress);
        }
    }

    ImGui::Separator();

    // Test pattern
    ImGui::Checkbox("Show Test Pattern", &show_test_pattern_);

    // Imagem - renderizada apenas na janela ImGui
    if (opengl_texture_id_ != 0) {
        ImVec2 imageSize(400, 300);
        // Inverter Y para corrigir orientação
        ImGui::Image((void*)(intptr_t)opengl_texture_id_, imageSize, ImVec2(0, 1), ImVec2(1, 0));

        // Mostrar cor de fundo para debug
        if (model_loaded_) {
            ImGui::Text("Background: Blue (model loaded)");
        } else {
            ImGui::Text("Background: Red (no model)");
        }
    } else {
        ImGui::Text("Texture not ready");
    }

    if (ImGui::Button("Fill Test Pattern")) {
        FillTestPattern();
        show_test_pattern_ = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Show Filament")) {
        show_test_pattern_ = false;
    }

    ImGui::Separator();

    // Controles de câmera com mais opções
    ImGui::Text("=== CAMERA CONTROLS ===");

    if (ImGui::SliderFloat("Distance", &camera_distance_, 0.1f, 50.0f)) {
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }

    if (ImGui::SliderFloat("Angle", &camera_angle_, 0.0f, 360.0f)) {
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }

    if (ImGui::SliderFloat("Height", &camera_height_, -10.0f, 10.0f)) {
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }

    // Botões de posição
    if (ImGui::Button("Very Close")) {
        camera_distance_ = 0.5f;
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        camera_distance_ = 2.0f;
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }
    ImGui::SameLine();
    if (ImGui::Button("Far")) {
        camera_distance_ = 10.0f;
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }
    ImGui::SameLine();
    if (ImGui::Button("Very Far")) {
        camera_distance_ = 50.0f;
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }

    // Botões para resetar câmera
    if (ImGui::Button("Reset Camera")) {
        camera_distance_ = 5.0f;
        camera_angle_ = 0.0f;
        camera_height_ = 0.0f;
        UpdateCamera(camera_distance_, camera_angle_, camera_height_);
    }

    // Botão para tentar recarregar modelo
    if (ImGui::Button("Reload Model")) {
        if (asset_) {
            scene_->removeEntities(asset_->getEntities(), asset_->getEntityCount());
            asset_loader_->destroyAsset(asset_);
            asset_ = nullptr;
        }
        model_loaded_ = false;
        LoadGLTFModel();
    }

    // Debug da câmera
    ImGui::Separator();
    ImGui::Text("=== CAMERA DEBUG ===");
    float radians = camera_angle_ * 3.14159f / 180.0f;
    float x = model_center_.x + camera_distance_ * sin(radians);
    float z = model_center_.z + camera_distance_ * cos(radians);
    ImGui::Text("Position: (%.3f, %.3f, %.3f)", x, model_center_.y + camera_height_, z);
    ImGui::Text("Looking at: (%.3f, %.3f, %.3f)", model_center_.x, model_center_.y, model_center_.z);
    ImGui::Text("Distance: %.3f", camera_distance_);

    // Debug de renderização
    ImGui::Separator();
    ImGui::Text("=== RENDER DEBUG ===");
    ImGui::Text("Render size: %dx%d", render_width_, render_height_);
    ImGui::Text("OpenGL Texture ID: %u", opengl_texture_id_);
    ImGui::Text("Framebuffer ID: %u", framebuffer_gl_id_);

    ImGui::End();
}

void SceneLayer::UpdateCamera(float distance, float angle, float height) {
    float radians = angle * 3.14159f / 180.0f;
    float x = model_center_.x + distance * sin(radians);
    float z = model_center_.z + distance * cos(radians);

    camera_->lookAt(
        {x, model_center_.y + height, z},
        {model_center_.x, model_center_.y, model_center_.z},
        {0, 1, 0}
    );
}

void SceneLayer::FillTestPattern() {
    std::vector<uint8_t> testData(render_width_ * render_height_ * 4);

    for (int y = 0; y < render_height_; ++y) {
        for (int x = 0; x < render_width_; ++x) {
            int index = (y * render_width_ + x) * 4;
            bool checker = ((x / 32) + (y / 32)) % 2;
            if (checker) {
                testData[index] = 255;   // R
                testData[index+1] = 0;   // G
                testData[index+2] = 0;   // B
            } else {
                testData[index] = 0;     // R
                testData[index+1] = 255; // G
                testData[index+2] = 0;   // B
            }
            testData[index+3] = 255;     // A
        }
    }

    glBindTexture(GL_TEXTURE_2D, opengl_texture_id_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, render_width_, render_height_,
                    GL_RGBA, GL_UNSIGNED_BYTE, testData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneLayer::OnDetach() {
    if (asset_) {
        scene_->removeEntities(asset_->getEntities(), asset_->getEntityCount());
        asset_loader_->destroyAsset(asset_);
        asset_ = nullptr;
    }

    delete resource_loader_;
    filament::gltfio::AssetLoader::destroy(&asset_loader_);

    if (render_target_) {
        engine_->destroy(render_target_);
    }
    if (color_texture_) {
        engine_->destroy(color_texture_);
    }
    engine_->destroy(view_);
    engine_->destroy(scene_);
    engine_->destroy(renderer_);
    engine_->destroy(swap_chain_);
    engine_->destroyCameraComponent(camera_entity_);

    if (opengl_texture_id_) {
        glDeleteTextures(1, &opengl_texture_id_);
    }
    if (framebuffer_gl_id_) {
        glDeleteFramebuffers(1, &framebuffer_gl_id_);
    }
    if (depth_buffer_id_) {
        glDeleteRenderbuffers(1, &depth_buffer_id_);
    }

    // Limpar buffer de pixels
    if (pixel_buffer_) {
        delete[] pixel_buffer_;
        pixel_buffer_ = nullptr;
    }

    filament::Engine::destroy(&engine_);
    Layer::OnDetach();
}

void SceneLayer::OnEvent(Event& e) {
    Layer::OnEvent(e);
}

} // namespace ProEngine