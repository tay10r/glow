#pragma once

#include <pybind11/pybind11.h>

namespace glow::python {

void
def_pfd_module(pybind11::module_&& m);

} // namespace glow::python
