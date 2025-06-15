#pragma once
#include "Core/Renderer/GraphicsContext.h"

#ifdef __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

namespace ProEngine {
class MetalContext : public GraphicsContext {
public:
    explicit MetalContext(void* windowHandle);
    void Init() override;
    void SwapBuffers() override;

private:
#ifdef __OBJC__
    id<CAMetalDrawable> current_drawable_ = nil;
    id<MTLDevice> device_ = nil;
    id<MTLCommandQueue> command_queue_ = nil;
    CAMetalLayer* metal_layer_ = nil;
#endif
    void* window_handle_ = nullptr;
};
}
