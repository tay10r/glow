#include <uikit/main.hpp>

#include <imgui.h>

#include <implot.h>

namespace {

class app_impl final : public uikit::app
{
public:
  void setup(uikit::platform& plt) override { plt.set_app_name("uikit_demo_app"); }

  void teardown() override {}

  void loop() override
  {
    ImGui::Begin("Window");

    if (ImPlot::BeginPlot("Example Plot")) {

      ImPlot::EndPlot();
    }

    ImGui::Button("Test");

    ImGui::End();
  }
};

} // namespace

namespace uikit {

auto
app::create() -> std::unique_ptr<app>
{
  return std::unique_ptr<app>(new app_impl());
}

} // namespace uikit
