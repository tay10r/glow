#pragma once

#include <GLES2/gl2.h>

#include <memory>

namespace uikit {

class viewport
{
public:
  virtual ~viewport() = default;

  virtual void bind() = 0;

  virtual void unbind() = 0;
};

class window_viewport : public viewport
{
public:
  static auto get() -> window_viewport&;

  virtual ~window_viewport() = default;
};

class offscreen_viewport : public viewport
{
public:
  virtual ~offscreen_viewport() = default;
};

class imgui_viewport : public viewport
{
public:
  static auto create() -> std::unique_ptr<imgui_viewport>;

  virtual ~imgui_viewport() = default;
};

} // namespace uikit
