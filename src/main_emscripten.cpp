#include <glow/main.hpp>

#include <emscripten.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <implot.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <glow/fonts.hpp>

#include <iostream>
#include <string>

#include <cstdlib>

#include "platform_base.h"

#include "../sago/platform_folders.h"

namespace {

class platform_impl final : public glow::platform_base
{
public:
  void build_fonts()
  {
    const float font_size{ 16 * m_scale };

    m_regular_font = glow::open_font("JetBrainsMonoNL-Regular.ttf", font_size);

    m_italic_font = glow::open_font("JetBrainsMonoNL-Italic.ttf", font_size);

    m_bold_font = glow::open_font("JetBrainsMonoNL-Bold.ttf", font_size);

    m_bold_italic_font = glow::open_font("JetBrainsMonoNL-BoldItalic.ttf", font_size);

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
    // Is there a better option, such as browser storage, that can be easily used here instead?
    return ".";
  }

  float get_scale() const override { return m_scale; }

  void set_scale(float scale) override
  {
    m_scale = scale;
    m_scale_changed = true;
  }

private:
  std::string m_app_name;

  float m_scale{ 1 };

  bool m_scale_changed{ false };

  ImFont* m_regular_font{ nullptr };

  ImFont* m_italic_font{ nullptr };

  ImFont* m_bold_font{ nullptr };

  ImFont* m_bold_italic_font{ nullptr };
};

struct loop_data final
{
  SDL_Window* window{ nullptr };

  glow::platform* plt{ nullptr };

  glow::app* app_instance{ nullptr };
};

} // namespace

int
main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window* window = SDL_CreateWindow(
    "Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  platform_impl plt;

  glClearColor(0, 0, 0, 1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 100");
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  auto& style = ImGui::GetStyle();
  style.WindowBorderSize = 0;
  style.WindowRounding = 2;
  style.FrameRounding = 2;

  auto* plot_context = ImPlot::CreateContext();

  plt.build_fonts();

  auto app = glow::app::create();

  app->setup(plt);

  SDL_SetWindowTitle(window, plt.get_app_name());

  const auto ui_path = plt.get_app_data_path() + "/ui.ini";

  io.IniFilename = ui_path.c_str();

  auto callback = [](void* loop_data_ptr) {
    auto* l_dat = static_cast<loop_data*>(loop_data_ptr);

    SDL_Window* window = l_dat->window;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    const auto& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClear(GL_COLOR_BUFFER_BIT);

    l_dat->app_instance->loop(*l_dat->plt);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
  };

  loop_data l_dat;
  l_dat.window = window;
  l_dat.plt = &plt;
  l_dat.app_instance = app.get();

  emscripten_set_main_loop_arg(callback, &l_dat, /* fps */ -1, /* simulate_infinite_loop = true */ 1);

  app->teardown(plt);

  app.reset();

  ImPlot::DestroyContext(plot_context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
