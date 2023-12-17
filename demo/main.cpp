#include <uikit/main.hpp>
#include <uikit/viewport.hpp>

#include <imgui.h>

#include <implot.h>

namespace {

class app_impl final : public uikit::app
{
public:
  void setup(uikit::platform& plt) override { plt.set_app_name("uikit_demo_app"); }

  void teardown(uikit::platform&) override {}

  void loop(uikit::platform& plt) override
  {
    ImGui::Begin("Window");

    if (ImPlot::BeginPlot("Example Plot")) {

      ImPlot::EndPlot();
    }

    if (ImGui::Button("Open File")) {
      plt.open_file_dialog("Example File Dialog", {}, nullptr, nullptr);
    }

    if (ImGui::Button("Open Directory")) {
      plt.open_directory_dialog("Example Directory Dialog", nullptr, nullptr);
    }

    ImGui::End();
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
