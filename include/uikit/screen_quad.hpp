#pragma once

#include <GLES2/gl2.h>

namespace uikit {

class screen_quad final
{
  GLuint id_{};

public:
  screen_quad();

  ~screen_quad();

  screen_quad(const screen_quad&) = delete;

  screen_quad(screen_quad&&) = delete;

  auto operator=(const screen_quad&) -> screen_quad& = delete;

  auto operator=(screen_quad&&) -> screen_quad& = delete;

  void draw(GLint position_attrib);
};

} // namespace uikit
