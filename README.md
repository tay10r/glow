About
=====

This is just a utility project to help quickly bring in dependencies that I usually use in my personal projects.
It also contains a little bit of OpenGL code to compile shaders, open nicer looking fonts in ImGui, store user settings in a sensible place, and get rid of entry point boiler plate.

Assuming you're fine with OpenGL ES 2 and GLFW (and all the other defaults), link to `uikit::main` and replace what you would otherwise put in `main` with this:

```cxx
#include <uikit/main.hpp>

#include <imgui.h>

// optionally
#include <GLES2/gl2.h>

namespace {

class app_impl final : public app
{
public:
  void setup(platform& plt) override { plt.set_app_name("My Demo App"); }
  void teardown() { }
  void loop() {
    ImGui::Begin("Test");
    ImGui::End();
  }
};

} // namespace

namespace uikit {

auto create_app() -> std::unique_ptr<app> { return new app_impl(); }

} // namespace uikit

```
