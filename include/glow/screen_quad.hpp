#pragma once

#include <GLES3/gl3.h>

namespace glow {

class screen_quad final
{
  GLuint buffer_{};

  GLuint vertex_array_{};

public:
  screen_quad();

  ~screen_quad();

  screen_quad(const screen_quad&) = delete;

  screen_quad(screen_quad&&) = delete;

  auto operator=(const screen_quad&) -> screen_quad& = delete;

  auto operator=(screen_quad&&) -> screen_quad& = delete;

  void draw(GLint position_attrib);
};

} // namespace glow
