#pragma once
#include "Core/Renderer/RendererAPI.h"

#ifdef __OBJC__
#import <Metal/Metal.h>
#endif

namespace ProEngine {
class MetalRendererAPI : public RendererAPI {
public:
    void Init() override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;
    void SetLineWidth(float width) override;

private:
#ifdef __OBJC__
    id<MTLCommandBuffer> command_buffer_ = nil;
#endif
};
}
