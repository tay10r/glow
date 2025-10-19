#pragma once

#include <GLES3/gl3.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace glow {

class shader_error : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;

  virtual ~shader_error() = default;
};

class shader_compile_error final : public shader_error
{
public:
  shader_compile_error(const std::string& what, const std::string& source);

  auto get_source() const -> const std::string&;

private:
  std::string m_source;
};

class shader_link_error final : public shader_error
{
public:
  using shader_error::shader_error;
};

/**
 * @brief Compiles a vertex and fragment shader into a single program.
 *
 * @return The ID of the linked program.
 *
 * @param defines A series of definitions to prepend to each shader string.
 *
 * @note This function will throw an exception if an error occurs.
 * */
GLuint
compile_shader(const char* vert_source,
               const char* frag_source,
               const std::vector<std::pair<std::string, std::string>>& defines);

} // namespace glow
