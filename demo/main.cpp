#include <glow/main.hpp>

#include <imgui.h>

#include <implot.h>

namespace {

class app_impl final : public glow::app
{
public:
  void setup(glow::platform& plt) override { plt.set_app_name("glow_demo_app"); }

  void teardown(glow::platform&) override {}

  void loop(glow::platform& plt) override
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
  void render_plots()
  {
    if (ImPlot::BeginPlot("Example Plot")) {
      ImPlot::EndPlot();
    }
  }

  void render_file_dialog_buttons(glow::platform& plt)
  {
    if (ImGui::Button("Open File")) {
      plt.open_file_dialog("Example File Dialog", {}, nullptr, nullptr);
    }

    if (ImGui::Button("Open Directory")) {
      plt.open_directory_dialog("Example Directory Dialog", nullptr, nullptr);
    }
  }
};

} // namespace

GLOW_APP(app_impl)
