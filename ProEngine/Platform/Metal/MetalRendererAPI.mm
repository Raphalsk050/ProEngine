#include "MetalRendererAPI.h"
#include "PEPCH.h"
#ifdef __APPLE__
#include <Metal/Metal.h>

namespace ProEngine {
static id<MTLDevice> s_device = nil;
static id<MTLCommandQueue> s_queue = nil;
static MTLViewport s_viewport;
static MTLClearColor s_clearColor = MTLClearColorMake(0,0,0,1);

void MetalRendererAPI::Init() {
    s_device = MTLCreateSystemDefaultDevice();
    s_queue = [s_device newCommandQueue];
}

void MetalRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    s_viewport.originX = x;
    s_viewport.originY = y;
    s_viewport.width = width;
    s_viewport.height = height;
    s_viewport.znear = 0.0;
    s_viewport.zfar = 1.0;
}

void MetalRendererAPI::SetClearColor(const glm::vec4& color) {
    s_clearColor = MTLClearColorMake(color.r, color.g, color.b, color.a);
}

void MetalRendererAPI::Clear() {
    id<MTLCommandBuffer> buffer = [s_queue commandBuffer];
    command_buffer_ = buffer;
    // Clearing handled when presenting frame by context
}

void MetalRendererAPI::SetLineWidth(float width) {
    // Not applicable for Metal sample
}

} // namespace ProEngine
#endif
