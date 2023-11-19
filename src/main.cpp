#include <uikit/main.hpp>

#include <GLFW/glfw3.h>

#include <GLES2/gl2.h>

#include <implot.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <uikit/imgui_fonts.hpp>

#include <iostream>
#include <string>

#include <cstdlib>

#include "../sago/platform_folders.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __unix__
#include <sys/stat.h>
#endif

namespace {

void
die(const char* msg)
{
  std::cerr << msg << std::endl;
  std::abort();
}

std::string app_name;

} // namespace

namespace uikit {

void
make_directory(const char* name)
{
#ifdef __linux__
  mkdir(name, 0755);
#else
  CreateDirectory(name, nullptr);
#endif
}

void
set_app_name(const char* name)
{
  app_name = name;
}

std::string
get_app_data_path()
{
  if (app_name.empty()) {
    return ".";
  }

  return sago::getDataHome() + "/" + app_name;
}

} // namespace uikit

#ifdef _WIN32
int
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#else
int
main(int argc, char** argv)
{
#endif
  auto* app = uikit::setup(argc, argv);

  const auto data_path = uikit::get_app_data_path();

  uikit::make_directory(data_path.c_str());

  const auto ui_path = data_path + "/ui.ini";

  if (glfwInit() != GLFW_TRUE) {
    die("Failed to initialize GLFW.");
    return EXIT_FAILURE;
  }

  int w{ 640 };
  int h{ 480 };

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  if (monitor) {
    const auto* video_mode = glfwGetVideoMode(monitor);
    if (video_mode) {
      w = video_mode->width;
      h = video_mode->height;
    }
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

  GLFWwindow* window = glfwCreateWindow(w, h, app_name.c_str(), nullptr, nullptr);
  if (!window) {
    die("Failed to create a window.");
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  glClearColor(0, 0, 0, 1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 100");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.IniFilename = data_path.c_str();
  auto& style = ImGui::GetStyle();
  style.WindowBorderSize = 0;
  style.WindowRounding = 2;
  style.FrameRounding = 2;

  uikit::open_font("fonts/Roboto-Medium.ttf", 16.0f);

  io.Fonts->Build();

  auto* plot_context = ImPlot::CreateContext();

  while (!glfwWindowShouldClose(window)) {

    glfwPollEvents();

    glfwMakeContextCurrent(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImPlot::SetCurrentContext(plot_context);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    int fb_w = 0;
    int fb_h = 0;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);

    glViewport(0, 0, fb_w, fb_h);
    glClear(GL_COLOR_BUFFER_BIT);

    uikit::loop(app);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  uikit::teardown(app);

  ImPlot::DestroyContext(plot_context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);

  glfwTerminate();

  return EXIT_SUCCESS;
}
