#include "MetalContext.h"
#include "PEPCH.h"
#ifdef __APPLE__
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <AppKit/AppKit.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace ProEngine {
MetalContext::MetalContext(void* windowHandle)
    : window_handle_(windowHandle) {}

void MetalContext::Init() {
    PENGINE_PROFILE_FUNCTION();
    device_ = MTLCreateSystemDefaultDevice();
    PENGINE_CORE_ASSERT(device_, "Failed to create Metal device");

    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = device_;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;

    GLFWwindow* window = static_cast<GLFWwindow*>(window_handle_);
    glfwMakeContextCurrent(window); // ensure window is current for resizing
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height){
        MetalContext* ctx = static_cast<MetalContext*>(glfwGetWindowUserPointer(win));
        if(ctx->metal_layer_) {
            ctx->metal_layer_.drawableSize = CGSizeMake(width, height);
        }
    });

    metal_layer_ = layer;
    NSWindow* nswindow = glfwGetCocoaWindow(window);
    nswindow.contentView.layer = layer;
    nswindow.contentView.wantsLayer = YES;

    command_queue_ = [device_ newCommandQueue];
}

void MetalContext::SwapBuffers() {
    PENGINE_PROFILE_FUNCTION();
    if(!metal_layer_) return;
    current_drawable_ = [metal_layer_ nextDrawable];
    id<MTLCommandBuffer> commandBuffer = [command_queue_ commandBuffer];
    [commandBuffer presentDrawable:current_drawable_];
    [commandBuffer commit];
}

} // namespace ProEngine
#endif
