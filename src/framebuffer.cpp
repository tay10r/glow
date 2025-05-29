#include <uikit/framebuffer.hpp>

namespace uikit {

framebuffer::framebuffer(const GLsizei width, const GLsizei height)
  : width_(width)
  , height_(height)
{
  glGenTextures(1, &color_attachment_);
  glBindTexture(GL_TEXTURE_2D, color_attachment_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  glGenFramebuffers(1, &id_);
  glBindFramebuffer(GL_FRAMEBUFFER, id_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);
  status_ = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

framebuffer::~framebuffer()
{
  glDeleteFramebuffers(1, &id_);

  glDeleteTextures(1, &color_attachment_);
}

auto
framebuffer::status() const -> GLenum
{
  return status_;
}

auto
framebuffer::color_attachment() -> GLuint
{
  return color_attachment_;
}

auto
framebuffer::width() const -> GLsizei
{
  return width_;
}

auto
framebuffer::height() const -> GLsizei
{
  return height_;
}

} // namespace uikit
