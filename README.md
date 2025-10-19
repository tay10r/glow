About
=====

This is just a utility project to help quickly bring in dependencies that I usually use in my personal projects.
It also contains a little bit of OpenGL code to compile shaders, open nicer looking fonts in ImGui, store user settings in a sensible place, and get rid of entry point boiler plate.
This library comes in two flavors: the C++ interface (for desktop and web browser platforms) and Python.
The two flavors are a bit different in some ways, but for the most part they are just a packaged interface to GLFW, OpenGL ES 2, ImGui and ImPlot.
There is little to no abstraction layers in place. If you already know the GLFW, OpenGL and ImGui APIs, learning to use this library is easy.

## C++ Interface

In order to use this project with C++, you'll need to compile it and link to it.
In order to compile it, you'll need to have CMake installed and an active `vcpkg` repository.
You'll need to set the environment variable `VCPKG_ROOT` to where the initialized `vcpkg` repository is.
Finally, you'll need to ensure you have the dependencies installed:

```
./vcpkg install glfw pybind11
```

Add this project as a sub directory in CMake either by installing it with `FetchContent` or by adding it as a git submodule and calling `add_subdirectory(glow)`.
Link to `glow::glow` and `glow::main` (contains the entry point code) and start your application like this:

```cxx
#include <glow/main.hpp>

#include <imgui.h>

// optionally
#include <GLES3/gl3.h>

namespace {

class app_impl final : public app
{
public:
  void setup(glow::platform& plt) override { plt.set_app_name("My Demo App"); }

  void teardown(glow::platform& plt) { }

  void loop(glow::platform& plt) {

    // In setup() you can disable auto-close with glow::platform::set_auto_close_enabled and allow the application to
    // decide when to exit.
    //
    // if (plt.exit_requested()) {
    //   plt.queue_exit();
    // }

    ImGui::Begin("Test");

    ImGui::End();
  }
};

} // namespace

// use this to register your derived `app` class as the entrypoint.
UIKIT_APP(app_impl)
```

## The Python Interface

You can also use this project in Python on both Linux and Windows.

Clone the repository and run:

```
pip install path/to/glow
```

Where `glow` is the path to the repository.
It works just fine in virtual environments as well.

In a Python file, you can import `glowpy` which contains submodules for OpenGL ES 3, GLFW, ImGui, ImPlot and portable-file-dialogs.

```python
import glow.gl as gl
import glow.glfw as glfw
import glow.imgui as imgui
import glow.implot as implot
import glow.pfd as pfd

def main():
  glfw.init()
  window = glfw.create_window()
  while not window.should_close():
    glfw.poll_events()
    window.begin_frame()
    if imgui.begin('Main Window'):
      if imgui.button('Click me!'):
        print('Button was clicked!')
    imgui.end()
    window.end_frame()
  window.close()
  glfw.terminate()
main()
```
