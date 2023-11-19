#pragma once

#include <string>

namespace uikit {

/// @brief Use this function to set the name of the application.
///
/// @param name The name to assign the application.
///
/// @note This affects where the data for the application is saved and what the window title is.
void
set_app_name(const char* name);

/// @brief Gets the path that application data can be stored in.
///
/// @return The path that application data can be stored in.
///
/// @note If @ref set_app_name is not called before this function, the data path will default to the current directory.
std::string
get_app_data_path();

/// @brief The entry point of the application.
///
/// @return A pointer to the application data to be passed to the other two functions.
void*
setup(int argc, char** argv);

/// @brief Called when the application is exiting, to clean up resources.
///
/// @param app_data The pointer that was returned by @ref setup.
void
teardown(void* app_data);

/// @brief Called when a frame is to be rendered.
///
/// @param app_data The pointer that was returned by @ref setup.
void
loop(void* app_data);

} // namespace uikit
