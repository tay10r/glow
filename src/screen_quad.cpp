#include <glow/screen_quad.hpp>

namespace glow {

screen_quad::screen_quad()
{
  const float vertices[] = {
    // clang-format off
    -1, -1,
     1, -1,
     1,  1,
     1,  1,
    -1,  1,
    -1, -1
    // clang-format on
  };

  glGenBuffers(1, &buffer_);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertex_array_);

  glBindVertexArray(vertex_array_);

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<const void*>(0));
}

screen_quad::~screen_quad()
{
  glDeleteVertexArrays(1, &vertex_array_);

  glDeleteBuffers(1, &buffer_);
}

void
screen_quad::draw(GLint position_attrib)
{
  glBindVertexArray(vertex_array_);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace glow
