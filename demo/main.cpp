#include <uikit/main.hpp>
#include <uikit/viewport.hpp>

#include <imgui.h>

#include <implot.h>

#include <algorithm>
#include <limits>

#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#include <cassert>

namespace {

class app_impl final : public uikit::app
{
public:
  void setup(uikit::platform& plt) override { plt.set_app_name("uikit_demo_app"); }

  void teardown(uikit::platform&) override {}

  void loop(uikit::platform& plt) override
  {
    if (plt.exit_requested()) {
      plt.queue_exit();
    }

    ImGui::Begin("Window");

    if (ImGui::BeginTabBar("Tabs")) {

      if (ImGui::BeginTabItem("Plot")) {

        render_plots();

        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("File Dialogs")) {

        render_file_dialog_buttons(plt);

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::End();
  }

protected:
  template<typename Scalar>
  static auto clamp(Scalar x, Scalar min, Scalar max) -> Scalar
  {
    return std::max(std::min(x, max), min);
  }

  void render_plots()
  {
    if (ImPlot::BeginPlot("Example Plot")) {
      ImPlot::EndPlot();
    }
  }

  void render_file_dialog_buttons(uikit::platform& plt)
  {
    if (ImGui::Button("Open File")) {
      plt.open_file_dialog("Example File Dialog", {}, nullptr, nullptr);
    }

    if (ImGui::Button("Open Directory")) {
      plt.open_directory_dialog("Example Directory Dialog", nullptr, nullptr);
    }
  }

private:
  std::unique_ptr<uikit::viewport> m_viewport;
};

} // namespace

namespace uikit {

auto
app::create() -> std::unique_ptr<app>
{
  return std::unique_ptr<app>(new app_impl());
}

} // namespace uikit
