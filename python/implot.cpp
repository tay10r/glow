#include "implot.hpp"

#include "gl.hpp"

#include <pybind11/numpy.h>

#include <implot.h>

#include <sstream>
#include <stdexcept>

#include <cstddef>
#include <cstdint>

namespace glow::python {

namespace {

namespace py = pybind11;

enum class Flags : std::uint32_t
{
  None = ImPlotFlags_None,
  NoTitle = ImPlotFlags_NoTitle,
  NoLegend = ImPlotFlags_NoLegend,
  NoMouseText = ImPlotFlags_NoMouseText,
  NoInputs = ImPlotFlags_NoInputs,
  NoMenus = ImPlotFlags_NoMenus,
  NoBoxSelect = ImPlotFlags_NoBoxSelect,
  NoFrame = ImPlotFlags_NoFrame,
  Equal = ImPlotFlags_Equal,
  Crosshairs = ImPlotFlags_Crosshairs,
  CanvasOnly = ImPlotFlags_CanvasOnly
};

auto
begin_plot(const char* label, const ImVec2& size, const Flags flags) -> bool
{
  return ImPlot::BeginPlot(label, size, static_cast<ImPlotFlags>(flags));
}

enum class LineFlags : std::uint32_t
{
  None = ImPlotLineFlags_None,
  Segments = ImPlotLineFlags_Segments,
  Loop = ImPlotLineFlags_Loop,
  SkipNaN = ImPlotLineFlags_SkipNaN,
  NoClip = ImPlotLineFlags_NoClip,
  Shaded = ImPlotLineFlags_Shaded
};

void
plot_line(const char* label, py::array_t<float, py::array::f_style | py::array::forcecast> data, const LineFlags flags)
{
  const auto* shape = data.shape();

  if (data.ndim() == 2) {
    ImPlot::PlotLine(label, data.data(), data.data() + shape[0], shape[0], static_cast<ImPlotLineFlags>(flags));
  } else if (data.ndim() == 1) {
    ImPlot::PlotLine(label, data.data(), static_cast<int>(shape[0]), 1.0, 0.0, static_cast<ImPlotLineFlags>(flags));
  } else {
    std::ostringstream stream;
    stream << "Line plots accept 1 or 2 dimensions, not " << data.ndim();
    throw std::runtime_error(stream.str());
  }
}

enum class ScatterFlags : std::uint32_t
{
  None = ImPlotScatterFlags_None,
  NoClip = ImPlotScatterFlags_NoClip
};

void
plot_scatter(const char* label,
             py::array_t<float, py::array::f_style | py::array::forcecast> data,
             const ScatterFlags flags)
{
  const auto* shape = data.shape();
  if (data.ndim() == 2) {
    ImPlot::PlotScatter(label, data.data(), data.data() + shape[1], shape[1], static_cast<ImPlotScatterFlags>(flags));
  } else if (data.ndim() == 1) {
    ImPlot::PlotScatter(label, data.data(), shape[0], 1.0, 0.0, static_cast<ImPlotScatterFlags>(flags));
  } else {
    std::ostringstream stream;
    stream << "Scatter plots accept 1 or 2 dimensions, not " << data.ndim();
    throw std::runtime_error(stream.str());
  }
}

enum class ImageFlags : ImPlotImageFlags
{
  None = ImPlotImageFlags_None
};

void
plot_image(const char* label,
           const gl_texture& tex,
           const ImPlotPoint& bounds_min,
           const ImPlotPoint& bounds_max,
           const ImVec2& uv0,
           const ImVec2& uv1,
           const ImVec4& tint_color,
           const ImageFlags flags)
{
  ImPlot::PlotImage(label,
                    reinterpret_cast<ImTextureID>(tex.id),
                    bounds_min,
                    bounds_max,
                    uv0,
                    uv1,
                    tint_color,
                    static_cast<ImPlotImageFlags>(flags));
}

} // namespace

void
def_implot_module(pybind11::module_&& m)
{
  py::enum_<Flags>(m, "Flags")
    .value("NONE", Flags::None)
    .value("NO_TITLE", Flags::NoTitle)
    .value("NO_LEGEND", Flags::NoLegend)
    .value("NO_MOUSE_TEXT", Flags::NoMouseText)
    .value("NO_INPUTS", Flags::NoInputs)
    .value("NO_MENUS", Flags::NoMenus)
    .value("NO_BOX_SELECT", Flags::NoBoxSelect)
    .value("NO_FRAME", Flags::NoFrame)
    .value("EQUAL", Flags::Equal)
    .value("CROSSHAIRS", Flags::Crosshairs)
    .value("CANVAS_ONLY", Flags::CanvasOnly)
    .def("__or__", [](const Flags left, const Flags right) -> Flags {
      return static_cast<Flags>(static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
    });

  m.def("begin_plot", &begin_plot, py::arg("label"), py::arg("size") = ImVec2(-1, 0), py::arg("flags") = Flags::None);
  m.def("end_plot", &ImPlot::EndPlot);

  py::class_<ImPlotPoint>(m, "Point")
    .def(py::init<double, double>(), py::arg("x") = 0.0, py::arg("y") = 0.0)
    .def_readwrite("x", &ImPlotPoint::x)
    .def_readwrite("y", &ImPlotPoint::y);

  py::enum_<LineFlags>(m, "LineFlags")
    .value("NONE", LineFlags::None)
    .value("SEGMENTS", LineFlags::Segments)
    .value("LOOP", LineFlags::Loop)
    .value("SKIP_NAN", LineFlags::SkipNaN)
    .value("NO_CLIP", LineFlags::NoClip)
    .value("SHADED", LineFlags::Shaded);
  m.def("plot_line", &plot_line, py::arg("label"), py::arg("data"), py::arg("flags") = LineFlags::None);

  py::enum_<ScatterFlags>(m, "ScatterFlags").value("NONE", ScatterFlags::None).value("NO_CLIP", ScatterFlags::NoClip);
  m.def("plot_scatter", &plot_scatter, py::arg("label"), py::arg("data"), py::arg("flags") = ScatterFlags::None);

  py::enum_<ImageFlags>(m, "ImageFlags").value("NONE", ImageFlags::None);
  m.def("plot_image",
        &plot_image,
        py::arg("label"),
        py::arg("texture"),
        py::arg("bounds_min"),
        py::arg("bounds_max"),
        py::arg("uv0") = ImVec2(0, 0),
        py::arg("uv1") = ImVec2(1, 1),
        py::arg("tint_color") = ImVec4(1, 1, 1, 1),
        py::arg("flags") = ImageFlags::None);
}

} // namespace glow::python
