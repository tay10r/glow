#include <uikit/screen_quad.hpp>

namespace uikit {

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

  glGenBuffers(1, &id_);

  glBindBuffer(GL_ARRAY_BUFFER, id_);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void
screen_quad::draw(GLint position_attrib)
{
  glBindBuffer(GL_ARRAY_BUFFER, id_);

  glEnableVertexAttribArray(position_attrib);

  glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<const void*>(0));

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace uikit
