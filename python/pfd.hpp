#pragma once

#include <pybind11/pybind11.h>

namespace uikit::python {

void
def_pfd_module(pybind11::module_&& m);

} // namespace uikit::python
