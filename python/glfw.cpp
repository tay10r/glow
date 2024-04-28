#include "glfw.hpp"

#include <uikit/fonts.hpp>

#include <pybind11/stl.h>

#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>

#include <optional>
#include <stdexcept>
#include <string>

namespace uikit::python {

namespace {

namespace py = pybind11;

auto
init() -> void
{
  if (glfwInit() != GLFW_TRUE) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
}

class monitor final
{
public:
  static auto primary() -> monitor { return monitor(glfwGetPrimaryMonitor()); }

  explicit monitor(GLFWmonitor* m)
    : m_monitor(m)
  {
  }

  auto ptr() -> GLFWmonitor* { return m_monitor; }

  auto name() const -> std::string { return std::string(glfwGetMonitorName(m_monitor)); }

private:
  GLFWmonitor* m_monitor{ nullptr };
};

class window final
{
public:
  window(int w, int h, const std::string& title, std::optional<monitor> mon, const float font_size)
  {
    GLFWmonitor* mon_ptr = mon.has_value() ? mon->ptr() : nullptr;

    m_window = glfwCreateWindow(w, h, title.c_str(), mon_ptr, nullptr);

    if (!m_window) {
      return;
    }

    glfwMakeContextCurrent(m_window);

    gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    IMGUI_CHECKVERSION();

    m_imgui_context = ImGui::CreateContext();

    ImGui_ImplOpenGL3_Init("#version 100");

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);

    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    uikit::open_font("JetBrainsMonoNL-Regular.ttf", font_size);

    ImGui::GetIO().Fonts->Build();

    m_implot_context = ImPlot::CreateContext();
  }

  window(const window&) = delete;

  window(window&&) = delete;

  auto operator=(const window&) -> window& = delete;

  auto operator=(window&&) -> window& = delete;

  ~window() { close(); }

  void set_imgui_config_path(const std::string& imgui_config_path)
  {
    if (m_imgui_context) {

      ImGui::SetCurrentContext(m_imgui_context);

      m_imgui_config_path = imgui_config_path;

      ImGui::GetIO().IniFilename = m_imgui_config_path.c_str();
    }
  }

  void set_title(const char* title) { glfwSetWindowTitle(m_window, title); }

  auto is_open() const -> bool { return m_window != nullptr; }

  void close()
  {
    if (!m_window) {
      return;
    }

    glfwMakeContextCurrent(m_window);
    ImGui::SetCurrentContext(m_imgui_context);

    ImPlot::DestroyContext(m_implot_context);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);

    m_window = nullptr;
  }

  auto should_close() const -> bool
  {
    if (!m_window) {
      return true;
    }

    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
  }

  void set_should_close(const bool should_close)
  {
    if (m_window) {
      glfwSetWindowShouldClose(m_window, should_close ? GLFW_TRUE : GLFW_FALSE);
    }
  }

  void begin_frame()
  {
    glfwMakeContextCurrent(m_window);

    ImGui::SetCurrentContext(m_imgui_context);

    ImPlot::SetCurrentContext(m_implot_context);

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      set_should_close(true);
    }

    ImGui::DockSpaceOverViewport();
  }

  void end_frame()
  {
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
  }

  auto get_framebuffer_size() const -> py::tuple
  {
    if (!m_window) {
      return py::make_tuple(0, 0);
    }

    int w = 0;
    int h = 0;
    glfwGetFramebufferSize(m_window, &w, &h);
    return py::make_tuple(w, h);
  }

private:
  GLFWwindow* m_window{ nullptr };

  ImGuiContext* m_imgui_context{ nullptr };

  ImPlotContext* m_implot_context{ nullptr };

  std::string m_imgui_config_path;
};

namespace py = pybind11;

} // namespace

void
def_glfw_module(py::module_&& m)
{
  m.def("init", init);
  m.def("terminate", glfwTerminate);

  m.def("get_primary_monitor", monitor::primary);
  py::class_<monitor>(m, "Monitor").def("name", &monitor::name);

  m.def("poll_events", glfwPollEvents);

  py::class_<window>(m, "Window")
    .def(py::init<int, int, std::string, std::optional<monitor>, float>(),
         py::arg("window_width"),
         py::arg("window_height"),
         py::arg("title"),
         py::arg("monitor") = py::none(),
         py::arg("font_size") = 16.0f)
    .def("close", &window::close)
    .def("set_imgui_config_path", &window::set_imgui_config_path, py::arg("config_path"))
    .def("begin_frame", &window::begin_frame)
    .def("end_frame", &window::end_frame)
    .def("is_open", &window::is_open)
    .def("should_close", &window::should_close)
    .def("set_should_close", &window::set_should_close, py::arg("should_close_flag"))
    .def("get_framebuffer_size", &window::get_framebuffer_size)
    .def("set_title", &window::set_title, py::arg("title"));
}

} // namespace uikit::python
