cmake_minimum_required(VERSION 3.14.7)

project(glow
  VERSION 0.3.0
  DESCRIPTION "A set of libraries for 3D application development."
  LANGUAGES C CXX
)

option(GLOW_PYTHON         "Whether or not to build the Python bindings."   OFF)
option(GLOW_INSTALL        "Whether or not to build the install rules."     OFF)
option(GLOW_BUILD_GLFW     "Whether or not to download and build GLFW."     OFF)
option(GLOW_BUILD_PYBIND11 "Whether or not to download and build pybind11." OFF)
option(GLOW_MAIN           "Whether or not to build the entry point code."  ON)

set(GLFW_URL     "https://github.com/glfw/glfw/archive/refs/tags/3.4.zip"                 CACHE STRING "The release URL of GLFW.")
set(IMGUI_URL    "https://github.com/ocornut/imgui/archive/refs/tags/v1.91.2-docking.zip" CACHE STRING "The release URL of ImGui.")
set(IMPLOT_URL   "https://github.com/epezent/implot/archive/refs/tags/v0.16.zip"          CACHE STRING "The release URL of ImPlot.")
set(PYBIND11_URL "https://github.com/pybind/pybind11/archive/refs/tags/v3.0.1.zip"        CACHE STRING "The release URL of pybind11.")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/overrides.cmake")
  include("${CMAKE_CURRENT_SOURCE_DIR}/overrides.cmake")
endif()

if(GLOW_PYTHON)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if(NOT EMSCRIPTEN)
  if(GLOW_BUILD_GLFW)
    include(cmake/GLFW.cmake)
  else()
    find_package(glfw3 CONFIG REQUIRED)
  endif()
  add_subdirectory(gles3)
endif()

include(cmake/ImGui.cmake)
include(cmake/ImPlot.cmake)

include(cmake/CMakeRC.cmake)
cmrc_add_resource_library(glow_font_data
  "fonts/JetBrainsMonoNL-Regular.ttf"
  "fonts/JetBrainsMonoNL-Italic.ttf"
  "fonts/JetBrainsMonoNL-Bold.ttf"
  "fonts/JetBrainsMonoNL-BoldItalic.ttf"
  WHENCE "fonts")

if(NOT EMSCRIPTEN)
  FetchContent_Declare(pfd URL "https://github.com/tay10r/portable-file-dialogs/archive/refs/tags/v1.0.zip")
  FetchContent_MakeAvailable(pfd)
endif()

add_library(glow STATIC
  include/glow/shader_compiler.hpp
  include/glow/fonts.hpp
  include/glow/framebuffer.hpp
  include/glow/screen_quad.hpp
  src/fonts.cpp
  src/shader_compiler.cpp
  src/framebuffer.cpp
  src/screen_quad.cpp)
target_include_directories(glow PUBLIC include)
target_link_libraries(glow
  PUBLIC
    glow_font_data
    imgui::imgui
    imgui::opengl3
    implot::implot)

if(EMSCRIPTEN)
  target_link_libraries(glow PUBLIC imgui::sdl2)
else()
  target_link_libraries(glow PUBLIC imgui::glfw glfw glow::gles3 portable_file_dialogs)
endif()

add_library(glow::glow ALIAS glow)

if(GLOW_PYTHON)
  add_subdirectory(python)
endif()

if(GLOW_MAIN)

  set(main_file)
  if(EMSCRIPTEN)
    set(main_file src/main_emscripten.cpp)
  else()
    set(main_file src/main.cpp)
  endif()

  add_library(glow_main STATIC
    ${main_file}
    include/glow/main.hpp
    sago/platform_folders.h
    sago/platform_folders.cpp
    src/platform_base.h
    src/platform_base.cpp)
  target_include_directories(glow_main PUBLIC include)
  target_link_libraries(glow_main PUBLIC glow::glow)

  add_library(glow::main ALIAS glow_main)

  if(EMSCRIPTEN)
    target_compile_options(glow_main PUBLIC "SHELL: -s USE_SDL=2")
    target_link_options(glow_main
      PUBLIC
        "SHELL: -s USE_SDL=2")
  endif()

endif()

if((CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR) AND GLOW_MAIN)
  add_executable(demo WIN32
    demo/main.cpp)
  target_link_libraries(demo PUBLIC glow::main)
endif()
