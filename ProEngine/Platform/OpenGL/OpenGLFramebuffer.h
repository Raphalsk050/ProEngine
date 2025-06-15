#pragma once

#include "Core/Renderer/Framebuffer.h"

namespace ProEngine {

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

private:
    void Invalidate();

    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_DepthAttachment = 0;
};

}
