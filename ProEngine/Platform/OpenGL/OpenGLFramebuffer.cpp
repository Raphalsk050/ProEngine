#include "PEPCH.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace ProEngine
{
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    {
        specification_ = spec;
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &renderer_id_);
        glDeleteTextures(1, &color_attachment_);
        glDeleteRenderbuffers(1, &depth_attachment_);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (renderer_id_ > -1)
        {
            glDeleteFramebuffers(1, &renderer_id_);
            glDeleteTextures(1, &color_attachment_);
            glDeleteRenderbuffers(1, &depth_attachment_);
        }

        glCreateFramebuffers(1, &renderer_id_);
        glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

        glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment_);
        glTextureStorage2D(color_attachment_, 1, GL_RGBA8, specification_.Width, specification_.Height);
        glTextureParameteri(color_attachment_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(color_attachment_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

        glCreateRenderbuffers(1, &depth_attachment_);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, specification_.Width, specification_.Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_attachment_);

        PENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
        glViewport(0, 0, specification_.Width, specification_.Height);
    }

    void OpenGLFramebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
            return;

        specification_.Width = width;
        specification_.Height = height;
        Invalidate();
    }
}
