#pragma once

#include <memory>
#include <string>

namespace uikit {

class platform
{
public:
  virtual ~platform() = default;

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
  ///
  /// @param app_data The pointer that was returned by @ref setup.
  virtual void teardown() = 0;

  /// @brief Called when a frame is to be rendered.
  virtual void loop() = 0;
};

} // namespace uikit
