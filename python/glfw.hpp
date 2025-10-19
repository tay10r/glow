#pragma once

#include <pybind11/pybind11.h>

namespace glow::python {

void
def_glfw_module(pybind11::module_&& m);

} // namespace glow::python
