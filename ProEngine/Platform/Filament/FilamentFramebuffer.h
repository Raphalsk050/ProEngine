#pragma once
#include "Core/Renderer/Framebuffer.h"
#include "Platform/Filament/FilamentEngine.h"

namespace ProEngine {

class FilamentFramebuffer : public Framebuffer {
public:
    explicit FilamentFramebuffer(const FramebufferSpecification& spec);
    ~FilamentFramebuffer() override;

    void Bind() const override;
    void Unbind() const override;
    void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetColorAttachmentRendererID() const override { return 0; }
    uint32_t GetEntityIDAttachmentRendererID() const override { return 0; }
    uint32_t GetDepthAttachmentRendererID() const override { return 0; }
    uint32_t GetRendererID() const override { return 0; }

    filament::Engine* GetEngine() const { return engine_->GetEngine(); }
    filament::Scene* GetScene() const { return engine_->GetScene(); }
    filament::View* GetView() const { return engine_->GetView(); }
    filament::Camera* GetCamera() const { return engine_->GetCamera(); }

private:
    void DestroyResources();

    FilamentEngine* engine_ = nullptr;
};

} // namespace ProEngine
