#pragma once

#include <GLES2/gl2.h>

namespace uikit {

class framebuffer final
{
  GLuint id_{};

  GLuint color_attachment_{};

  GLsizei width_{};

  GLsizei height_{};

  GLenum status_{};

public:
  framebuffer(GLsizei width, GLsizei height);

  ~framebuffer();

  framebuffer(const framebuffer&) = delete;

  framebuffer(framebuffer&&) = delete;

  auto operator=(const framebuffer&) -> framebuffer& = delete;

  auto operator=(framebuffer&&) -> framebuffer& = delete;

  [[nodiscard]] auto status() const -> GLenum;

  void bind(GLenum target = GL_FRAMEBUFFER);

  [[nodiscard]] auto color_attachment() -> GLuint;

  [[nodiscard]] auto width() const -> GLsizei;

  [[nodiscard]] auto height() const -> GLsizei;
};

} // namespace uikit
