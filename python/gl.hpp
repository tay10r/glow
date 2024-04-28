#pragma once

#include <pybind11/pybind11.h>

#include <glad/glad.h>

namespace uikit::python {

template<typename Derived>
struct gl_object
{
  GLuint id{};

  virtual ~gl_object() = default;
};

struct gl_buffer final : public gl_object<gl_buffer>
{};

struct gl_shader final : public gl_object<gl_shader>
{};

struct gl_program final : public gl_object<gl_program>
{};

struct gl_texture final : public gl_object<gl_texture>
{};

void
def_gl_module(pybind11::module_&& m);

} // namespace uikit::python
