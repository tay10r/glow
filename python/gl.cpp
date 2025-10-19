#include "gl.hpp"

#include <glad/glad.h>

#include <pybind11/stl.h>

#include <string_view>
#include <vector>

#include <cstddef>

namespace glow::python {

namespace {

namespace py = pybind11;

void
viewport(GLint x, GLint y, GLsizei w, GLsizei h)
{
  glViewport(x, y, w, h);
}

void
clear(GLbitfield flags)
{
  glClear(flags);
}

void
clear_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  glClearColor(r, g, b, a);
}

enum class Error : GLenum
{
  NoError = GL_NO_ERROR,
  InvalidEnum = GL_INVALID_ENUM,
  InvalidValue = GL_INVALID_VALUE,
  InvalidOperation = GL_INVALID_OPERATION,
  InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
  OutOfMemory = GL_OUT_OF_MEMORY
};

auto
get_error() -> Error
{
  return static_cast<Error>(glGetError());
}

auto
gen_textures(std::size_t n) -> std::vector<gl_texture>
{
  std::vector<GLuint> ids(n);
  glGenTextures(static_cast<GLsizei>(n), ids.data());

  std::vector<gl_texture> textures(n);
  for (std::size_t i = 0; i < n; i++) {
    textures[i].id = ids[i];
  }

  return textures;
}

void
delete_textures(const std::vector<gl_texture>& textures)
{
  std::vector<GLuint> ids(textures.size());
  for (std::size_t i = 0; i < textures.size(); i++) {
    ids[i] = textures[i].id;
  }

  glDeleteTextures(static_cast<GLsizei>(ids.size()), ids.data());
}

enum class TextureTarget : GLenum
{
  Texture2D = GL_TEXTURE_2D,
  CubeMap = GL_TEXTURE_CUBE_MAP,
  CubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  CubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  CubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  CubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  CubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  CubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

void
bind_texture(TextureTarget target, const gl_texture& tex)
{
  glBindTexture(static_cast<GLenum>(target), tex.id);
}

enum class TextureIntParameter
{
  MinFilter = GL_TEXTURE_MIN_FILTER,
  MagFilter = GL_TEXTURE_MAG_FILTER,
  WrapS = GL_TEXTURE_WRAP_S,
  WrapT = GL_TEXTURE_WRAP_T
};

void
set_texture_int_parameter(TextureTarget target, TextureIntParameter param, GLint value)
{
  glTexParameteri(static_cast<GLenum>(target), static_cast<GLenum>(param), value);
}

enum class TextureMinFunction : GLint
{
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR,
  NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
  LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
  NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
  LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFunction : GLint
{
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR
};

enum class TextureWrapMode : GLint
{
  ClampToEdge = GL_CLAMP_TO_EDGE,
  MirroredRepeat = GL_MIRRORED_REPEAT,
  Repeat = GL_REPEAT
};

enum class TextureFormat : GLint
{
  Alpha = GL_ALPHA,
  Luminance = GL_LUMINANCE,
  LuminanceAlpha = GL_LUMINANCE_ALPHA,
  RGB = GL_RGB,
  RGBA = GL_RGBA
};

enum class TexelType : GLenum
{
  UnsignedByte = GL_UNSIGNED_BYTE,
  UnsignedShort565 = GL_UNSIGNED_SHORT_5_6_5,
  UnsignedShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
  UnsignedShort5551 = GL_UNSIGNED_SHORT_5_5_5_1
};

void
tex_image_2d(TextureTarget target,
             GLint level,
             TextureFormat internal_format,
             GLsizei width,
             GLsizei height,
             GLint border,
             TextureFormat format,
             TexelType type,
             py::bytes& data)
{
  std::string_view data_view(data);

  glTexImage2D(static_cast<GLenum>(target),
               level,
               static_cast<GLint>(internal_format),
               width,
               height,
               border,
               static_cast<GLenum>(format),
               static_cast<GLenum>(type),
               data_view.data());
}

void
tex_sub_image_2d(TextureTarget target,
                 GLint level,
                 GLint xoffset,
                 GLint yoffset,
                 GLsizei width,
                 GLsizei height,
                 TextureFormat format,
                 TexelType type,
                 py::bytes& data)
{
  glTexSubImage2D(static_cast<GLenum>(target),
                  level,
                  xoffset,
                  yoffset,
                  width,
                  height,
                  static_cast<GLenum>(format),
                  static_cast<GLenum>(format),
                  std::string_view(data).data());
}

} // namespace

void
def_gl_module(pybind11::module_&& mod)
{
  py::enum_<Error>(mod, "Error")
    .value("NO_ERROR", Error::NoError)
    .value("INVALID_ENUM", Error::InvalidEnum)
    .value("INVALID_OPERATION", Error::InvalidOperation)
    .value("INVALID_FRAMEBUFFER_OPERATION", Error::InvalidFramebufferOperation)
    .value("OUT_OF_MEMORY", Error::OutOfMemory);

  py::enum_<TextureTarget>(mod, "TextureTarget")
    .value("TEXTURE_2D", TextureTarget::Texture2D)
    .value("TEXTURE_CUBE_MAP", TextureTarget::CubeMap)
    .value("TEXTURE_CUBE_MAP_POSITIVE_X", TextureTarget::CubeMapPositiveX)
    .value("TEXTURE_CUBE_MAP_NEGATIVE_X", TextureTarget::CubeMapNegativeX)
    .value("TEXTURE_CUBE_MAP_POSITIVE_Y", TextureTarget::CubeMapPositiveY)
    .value("TEXTURE_CUBE_MAP_NEGATIVE_Y", TextureTarget::CubeMapNegativeY)
    .value("TEXTURE_CUBE_MAP_POSITIVE_Z", TextureTarget::CubeMapPositiveZ)
    .value("TEXTURE_CUBE_MAP_NEGATIVE_Z", TextureTarget::CubeMapNegativeZ);

  py::enum_<TextureMinFunction>(mod, "TextureMinFunction")
    .value("NEAREST", TextureMinFunction::Nearest)
    .value("LINEAR", TextureMinFunction::Linear)
    .value("NEAREST_MIPMAP_NEAREST", TextureMinFunction::NearestMipmapNearest)
    .value("LINEAR_MIPMAP_NEAREST", TextureMinFunction::LinearMipmapNearest)
    .value("NEAREST_MIPMAP_LINEAR", TextureMinFunction::NearestMipmapLinear)
    .value("LINEAR_MIPMAP_LINEAR", TextureMinFunction::LinearMipmapLinear);

  py::enum_<TextureMagFunction>(mod, "TextureMagFunction")
    .value("NEAREST", TextureMagFunction::Nearest)
    .value("LINEAR", TextureMagFunction::Linear);

  py::enum_<TextureWrapMode>(mod, "TextureWrapMode")
    .value("CLAMP_TO_EDGE", TextureWrapMode::ClampToEdge)
    .value("MIRRORED_REPEAT", TextureWrapMode::MirroredRepeat)
    .value("GL_REPEAT", TextureWrapMode::Repeat);

  py::enum_<TextureFormat>(mod, "TextureFormat")
    .value("ALPHA", TextureFormat::Alpha)
    .value("LUMINANCE", TextureFormat::Luminance)
    .value("LUMINANCE_ALPHA", TextureFormat::LuminanceAlpha)
    .value("RGB", TextureFormat::RGB)
    .value("RGBA", TextureFormat::RGBA);

  py::enum_<TexelType>(mod, "TexelType")
    .value("UNSIGNED_BYTE", TexelType::UnsignedByte)
    .value("UNSIGNED_SHORT_5_6_5", TexelType::UnsignedShort565)
    .value("UNSIGNED_SHORT_4_4_4_4", TexelType::UnsignedShort4444)
    .value("UNSIGNED_SHORT_5_5_5_1", TexelType::UnsignedShort5551);

  py::class_<gl_texture>(mod, "Texture");

  mod.attr("COLOR_BUFFER_BIT") = GL_COLOR_BUFFER_BIT;
  mod.attr("DEPTH_BUFFER_BIT") = GL_DEPTH_BUFFER_BIT;

  mod.def("get_error", &get_error);
  mod.def("viewport", &viewport, py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"));
  mod.def("clear", &clear, py::arg("flags"));
  mod.def("clear_color", &clear_color, py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"));
  mod.def("gen_textures", &gen_textures, py::arg("num_textures"));
  mod.def("delete_textures", &delete_textures, py::arg("textures"));
  mod.def("bind_texture", &bind_texture, py::arg("target"), py::arg("texture"));
  mod.def(
    "set_texture_int_parameter", &set_texture_int_parameter, py::arg("target"), py::arg("param"), py::arg("value"));
  mod.def("set_active_texture", [](GLint index) { glActiveTexture(GL_TEXTURE0 + index); }, py::arg("index"));
  mod.def("tex_image_2d",
          &tex_image_2d,
          py::arg("target"),
          py::arg("level"),
          py::arg("internal_format"),
          py::arg("width"),
          py::arg("height"),
          py::arg("border"),
          py::arg("format"),
          py::arg("type"),
          py::arg("data"));
  mod.def("tex_sub_image_2d",
          &tex_sub_image_2d,
          py::arg("target"),
          py::arg("level"),
          py::arg("x"),
          py::arg("y"),
          py::arg("width"),
          py::arg("height"),
          py::arg("format"),
          py::arg("type"),
          py::arg("data"));
}

} // namespace glow::python
