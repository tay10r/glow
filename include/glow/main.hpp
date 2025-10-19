#pragma once

#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

namespace glow {

class platform
{
public:
  virtual ~platform() = default;

  virtual void set_auto_close_enabled(bool enabled) {}

  virtual auto exit_requested() -> bool { return false; }

  virtual void queue_exit() {}

  /// @brief Use this function to set the name of the application.
  ///
  /// @param name The name to assign the application.
  ///
  /// @note This affects where the data for the application is saved and what the window title is.
  virtual void set_app_name(const char* name) = 0;

  /// @brief Gets the path that application data can be stored in.
  ///
  /// @return The path that application data can be stored in.
  ///
  /// @note If @ref set_app_name is not called before this function, the data path will default to the current
  /// directory.
  virtual auto get_app_data_path() const -> std::string = 0;

  /// @brief Gets the current scale factor.
  virtual auto get_scale() const -> float = 0;

  /// @brief Sets the current scale factor.
  ///
  /// @note This is not a cheap function call, since it requires rebuilding the font atlas.
  virtual void set_scale(float scale) = 0;

  virtual auto get_regular_font() -> ImFont* = 0;

  virtual auto get_italic_font() -> ImFont* = 0;

  virtual auto get_bold_font() -> ImFont* = 0;

  virtual auto get_bold_italic_font() -> ImFont* = 0;

  virtual void open_directory_dialog(const char* title,
                                     void* cb_data,
                                     void (*cb_func)(void*, const char* directory_path))
  {
  }

  virtual void open_file_dialog(const char* title,
                                const std::vector<std::string>& filters,
                                void* cb_data,
                                void (*cb_func)(void*, const char* file_path))
  {
  }

  virtual void save_file_dialog(const char* title, void* cb_data, void (*cb_func)(void*, const char* file_path)) {}
};

class app
{
public:
  /// @note This should be defined by the library user, in order to create the application.
  static auto create() -> std::unique_ptr<app>;

  virtual ~app() = default;

  /// @brief The entry point of the application.
  virtual void setup(platform&) = 0;

  /// @brief Called when the application is exiting, to clean up resources.
  virtual void teardown(platform&) = 0;

  /// @brief Called when a frame is to be rendered.
  virtual void loop(platform&) = 0;
};

} // namespace glow

/**
 * @brief Use this macro to set a derived @ref app class as the class that gets
 *        instantiated for the program.
 *
 * @details This macro can optionally be used to reduce some of the boiler plate code for instantiating the
 *          user defined application class. All it does is implement the function in the app base class that
 *          instantiates the derived one.
 *
 * @note Invoke this macro in the global namespace, do not invoke it in a nested namespace or class namespace.
 * */
#define GLOW_APP(app_impl)                                                                                             \
  namespace glow {                                                                                                     \
  auto app::create() -> std::unique_ptr<app>                                                                           \
  {                                                                                                                    \
    return std::make_unique<app_impl>();                                                                               \
  }                                                                                                                    \
  } // namespace glow

