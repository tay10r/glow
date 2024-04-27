#include <pybind11/pybind11.h>

#include "gl.hpp"
#include "glfw.hpp"
#include "imgui.hpp"
#include "implot.hpp"
#include "pfd.hpp"

PYBIND11_MODULE(uikitpy, m)
{
  uikit::python::def_glfw_module(m.def_submodule("glfw", "Bindings to the GLFW library."));
  uikit::python::def_gl_module(m.def_submodule("gl", "Bindings to the OpenGL ES 2 library."));
  uikit::python::def_imgui_module(m.def_submodule("imgui", "Bindings to the ImGui library."));
  uikit::python::def_implot_module(m.def_submodule("implot", "Bindings to the ImPlot library."));
  uikit::python::def_pfd_module(m.def_submodule("pfd", "Bindings to the Portable File Dialogs library."));
}
