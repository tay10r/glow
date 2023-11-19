#include <uikit/main.hpp>

#include <imgui.h>

#include <implot.h>

namespace uikit {

void*
setup(int, char**)
{
  set_app_name("uikit_demo_app");

  return nullptr;
}

void
teardown(void*)
{
}

void
loop(void*)
{
  ImGui::Begin("Window");

  if (ImPlot::BeginPlot("Example Plot")) {

    ImPlot::EndPlot();
  }

  ImGui::Button("Test");

  ImGui::End();
}

} // namespace uikit
