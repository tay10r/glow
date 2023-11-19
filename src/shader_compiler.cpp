#include <uikit/shader_compiler.hpp>

#include <cstring>

namespace uikit {

shader_compile_error::shader_compile_error(const std::string& what, const std::string& source)
  : shader_error(what)
  , m_source(source)
{
}

auto
shader_compile_error::get_source() const -> const std::string&
{
  return m_source;
}

namespace {

GLuint
compile_single_shader(const char* source, GLenum type)
{
  GLuint id = glCreateShader(type);

  const GLint size = std::strlen(source);

  glShaderSource(id, 1, &source, &size);

  glCompileShader(id);

  GLint status{ GL_COMPILE_STATUS };

  glGetShaderiv(id, GL_COMPILE_STATUS, &status);

  if (status == GL_TRUE) {
    return id;
  }

  GLint log_length{ 0 };

  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

  GLsizei read_size{ 0 };

  std::string log;

  log.resize(log_length);

  glGetShaderInfoLog(id, log_length, &read_size, &log[0]);

  log.resize(read_size);

  glDeleteShader(id);

  throw shader_compile_error(log, source);
}

} // namespace

GLuint
compile_shader(const char* vert_source, const char* frag_source)
{
  GLuint vert_id = compile_single_shader(vert_source, GL_VERTEX_SHADER);

  GLuint frag_id{ 0 };

  try {
    frag_id = compile_single_shader(frag_source, GL_FRAGMENT_SHADER);
  } catch (const shader_compile_error& e) {
    throw e;
  }

  GLuint id = glCreateProgram();

  glAttachShader(id, vert_id);
  glAttachShader(id, frag_id);

  glLinkProgram(id);

  glDetachShader(id, vert_id);
  glDetachShader(id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  GLint link_status{ GL_FALSE };

  glGetProgramiv(id, GL_LINK_STATUS, &link_status);

  if (link_status == GL_TRUE) {
    return link_status;
  }

  GLint log_length{ 0 };

  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);

  GLsizei read_size{ 0 };

  std::string log;

  log.resize(log_length);

  glGetProgramInfoLog(id, log_length, &read_size, &log[0]);

  log.resize(read_size);

  glDeleteProgram(id);

  throw shader_link_error(log);
}

} // namespace uikit
