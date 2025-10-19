#include <pybind11/pybind11.h>

#include "gl.hpp"
#include "glfw.hpp"
#include "imgui.hpp"
#include "implot.hpp"
#include "pfd.hpp"

PYBIND11_MODULE(glowpy, m)
{
  glow::python::def_glfw_module(m.def_submodule("glfw", "Bindings to the GLFW library."));
  glow::python::def_gl_module(m.def_submodule("gl", "Bindings to the OpenGL ES 2 library."));
  glow::python::def_imgui_module(m.def_submodule("imgui", "Bindings to the ImGui library."));
  glow::python::def_implot_module(m.def_submodule("implot", "Bindings to the ImPlot library."));
  glow::python::def_pfd_module(m.def_submodule("pfd", "Bindings to the Portable File Dialogs library."));
}
