#include <uikit/main.hpp>

#include <GLFW/glfw3.h>

#include <GLES2/gl2.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>

#include <uikit/fonts.hpp>

#include <iostream>
#include <string>

#include <cstdlib>

#include "../sago/platform_folders.h"

#include <portable-file-dialogs.h>

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

class dialog
{
public:
  virtual ~dialog() = default;

  virtual void cancel() = 0;

  virtual bool try_complete() = 0;
};

class directory_dialog final : public dialog
{
public:
  directory_dialog(const char* title, void* cb_data, void (*cb_func)(void*, const char* path))
    : m_cb_data(cb_data)
    , m_cb_func(cb_func)
    , m_dialog(title)
  {
  }

  void cancel() override { m_dialog.kill(); }

  bool try_complete() override
  {
    if (!m_dialog.ready()) {
      return false;
    }

    const auto path = m_dialog.result();
    if (m_cb_func) {
      m_cb_func(m_cb_data, path.c_str());
    }

    return true;
  }

private:
  void* m_cb_data{ nullptr };

  void (*m_cb_func)(void*, const char*){ nullptr };

  pfd::select_folder m_dialog;
};

class open_file_dialog final : public dialog
{
public:
  open_file_dialog(const char* title,
                   const std::vector<std::string>& filters,
                   void* cb_data,
                   void (*cb_func)(void*, const char*))
    : m_dialog(title, "", filters)
    , m_cb_data(cb_data)
    , m_cb_func(cb_func)
  {
  }

  void cancel() override { m_dialog.kill(); }

  bool try_complete() override
  {
    if (!m_dialog.ready()) {
      return false;
    }

    const auto results = m_dialog.result();

    if (m_cb_func) {
      for (const auto& path : results) {
        m_cb_func(m_cb_data, path.c_str());
      }
    }

    return true;
  }

private:
  pfd::open_file m_dialog;

  void* m_cb_data{ nullptr };

  void (*m_cb_func)(void* cb_data, const char* path){ nullptr };
};

class platform_impl final : public uikit::platform
{
public:
  void build_fonts()
  {
    const float font_size{ 16 * m_scale };

    m_regular_font = uikit::open_font("JetBrainsMonoNL-Regular.ttf", font_size);

    m_italic_font = uikit::open_font("JetBrainsMonoNL-Italic.ttf", font_size);

    m_bold_font = uikit::open_font("JetBrainsMonoNL-Bold.ttf", font_size);

    m_bold_italic_font = uikit::open_font("JetBrainsMonoNL-BoldItalic.ttf", font_size);

    ImGui::GetIO().Fonts->Build();
  }

  auto get_regular_font() -> ImFont* override { return m_regular_font; }

  auto get_italic_font() -> ImFont* override { return m_italic_font; }

  auto get_bold_font() -> ImFont* override { return m_bold_font; }

  auto get_bold_italic_font() -> ImFont* override { return m_bold_italic_font; }

  auto get_app_name() const -> const char* { return m_app_name.c_str(); }

  void set_app_name(const char* name) override { m_app_name = name; }

  auto get_app_data_path() const -> std::string override
  {
    if (m_app_name.empty()) {
      return ".";
    }

    return sago::getDataHome() + "/" + m_app_name;
  }

  void make_data_directory()
  {
#if defined(__linux__) || defined(__EMSCRIPTEN__)
    mkdir(get_app_data_path().c_str(), 0755);
#elif _WIN32
    CreateDirectory(get_app_data_path().c_str(), nullptr);
#endif
  }

  float get_scale() const override { return m_scale; }

  void set_scale(float scale) override
  {
    m_scale = scale;
    m_scale_changed = true;
  }

  void poll_dialog()
  {
    if (!m_dialog) {
      return;
    }

    if (!m_dialog->try_complete()) {
      return;
    }

    m_dialog.reset();
  }

  void open_directory_dialog(const char* title,
                             void* cb_data,
                             void (*cb_func)(void*, const char* directory_path)) override
  {
    if (m_dialog) {
      m_dialog->cancel();
    }

    m_dialog = std::make_unique<directory_dialog>(title, cb_data, cb_func);
  }

  void open_file_dialog(const char* title,
                        const std::vector<std::string>& filters,
                        void* cb_data,
                        void (*cb_func)(void*, const char* directory_path)) override
  {
    if (m_dialog) {
      m_dialog->cancel();
    }

    m_dialog = std::make_unique<::open_file_dialog>(title, filters, cb_data, cb_func);
  }

private:
  std::string m_app_name;

  float m_scale{ 1 };

  bool m_scale_changed{ false };

  ImFont* m_regular_font{ nullptr };

  ImFont* m_italic_font{ nullptr };

  ImFont* m_bold_font{ nullptr };

  ImFont* m_bold_italic_font{ nullptr };

  std::unique_ptr<dialog> m_dialog;
};

} // namespace

#ifdef _WIN32
int
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#else
int
main(int argc, char** argv)
{
#endif
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

  GLFWwindow* window = glfwCreateWindow(w, h, "", nullptr, nullptr);
  if (!window) {
    die("Failed to create a window.");
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  platform_impl plt;

  glClearColor(0, 0, 0, 1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 100");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  auto& style = ImGui::GetStyle();
  style.WindowBorderSize = 0;
  style.WindowRounding = 2;
  style.FrameRounding = 2;

  auto* plot_context = ImPlot::CreateContext();

  plt.build_fonts();

  auto app = uikit::app::create();

  app->setup(plt);

  glfwSetWindowTitle(window, plt.get_app_name());

  plt.make_data_directory();

  const auto ui_path = plt.get_app_data_path() + "/ui.ini";

  io.IniFilename = ui_path.c_str();

  while (!glfwWindowShouldClose(window)) {

    glfwPollEvents();

    plt.poll_dialog();

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

    app->loop(plt);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  app->teardown(plt);

  app.reset();

  ImPlot::DestroyContext(plot_context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);

  glfwTerminate();

  return EXIT_SUCCESS;
}
