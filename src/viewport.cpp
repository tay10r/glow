#include <uikit/viewport.hpp>

#include <imgui.h>

namespace uikit {

namespace {

class window_viewport_impl final : public window_viewport
{
public:
  void bind() override { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  void unbind() override {}
};

#if 0
class offscreen_viewport_impl final : public offscreen_viewport
{
  public:
    void bind() override{}
    void unbind() override {}
};
#endif

class imgui_viewport_impl final : public imgui_viewport
{
public:
  void bind() override {}

  void unbind() override
  {

    // ImGui::ImageButton(reinterpret_cast<ImTextureID>(
  }
};

} // namespace

auto
imgui_viewport::create() -> std::unique_ptr<imgui_viewport>
{
  return std::unique_ptr<imgui_viewport>(new imgui_viewport_impl());
}

} // namespace uikit
