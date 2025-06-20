#include "Platform/Filament/FilamentFramebuffer.h"
#include "Platform/Filament/FilamentEngine.h"

namespace ProEngine {

FilamentFramebuffer::FilamentFramebuffer(const FramebufferSpecification& spec) {
    specification_ = spec;
    engine_ = new FilamentEngine();
    engine_->Initialize(spec.Width, spec.Height);
}

FilamentFramebuffer::~FilamentFramebuffer() {
    DestroyResources();
}

void FilamentFramebuffer::DestroyResources() {
    if (engine_) {
        engine_->Destroy();
        delete engine_;
        engine_ = nullptr;
    }
}

void FilamentFramebuffer::Bind() const {
    auto* renderer = engine_->GetRenderer();
    auto* swap = engine_->GetEngine()->createSwapChain(nullptr);
    if (renderer->beginFrame(swap)) {
        renderer->render(engine_->GetView());
        renderer->endFrame();
    }
    engine_->GetEngine()->destroy(swap);
}

void FilamentFramebuffer::Unbind() const {
    // Nothing to do for pure Filament
}

void FilamentFramebuffer::Resize(uint32_t width, uint32_t height) {
    specification_.Width = width;
    specification_.Height = height;
    DestroyResources();
    engine_ = new FilamentEngine();
    engine_->Initialize(width, height);
}

} // namespace ProEngine
