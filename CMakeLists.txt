cmake_minimum_required(VERSION 3.14.7)

project(uikit)

include(cmake/CMakeRC.cmake)

set(glfw_url   "https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip"               CACHE STRING "The release URL of GLFW.")
set(imgui_url  "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9-docking.zip" CACHE STRING "The release URL of ImGui.")
set(implot_url "https://github.com/epezent/implot/archive/refs/tags/v0.16.zip"          CACHE STRING "The release URL of ImPlot.")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/overrides.cmake")
  include("${CMAKE_CURRENT_SOURCE_DIR}/overrides.cmake")
endif()

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

if(NOT EMSCRIPTEN)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw URL "${glfw_url}")
  FetchContent_MakeAvailable(glfw)
  add_subdirectory(gles2)
endif()

FetchContent_Declare(imgui URL "${imgui_url}")
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)

add_library(imgui STATIC
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
  "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}" "${imgui_SOURCE_DIR}/misc/cpp")
target_compile_definitions(imgui PUBLIC ImDrawIdx=unsigned)
add_library(imgui::imgui ALIAS imgui)

if(EMSCRIPTEN)
else()
  add_library(imgui_glfw STATIC
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
  target_include_directories(imgui_glfw PUBLIC "${imgui_SOURCE_DIR}/backends")
  target_link_libraries(imgui_glfw PUBLIC imgui::imgui glfw)
  target_compile_definitions(imgui_glfw PUBLIC GLFW_INCLUDE_NONE=1)
  add_library(imgui::glfw ALIAS imgui_glfw)
endif()

add_library(imgui_opengl3 STATIC
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")
target_include_directories(imgui_opengl3 PUBLIC "${imgui_SOURCE_DIR}/backends")
target_link_libraries(imgui_opengl3 PUBLIC imgui::imgui)
if(NOT EMSCRIPTEN)
  target_link_libraries(imgui_opengl3 PUBLIC glad::glad)
endif()
target_compile_definitions(imgui_opengl3 PUBLIC IMGUI_IMPL_OPENGL_ES2=1)
add_library(imgui::opengl3 ALIAS imgui_opengl3)

cmrc_add_resource_library(uikit_font_data
  "fonts/JetBrainsMonoNL-Regular.ttf"
  "fonts/JetBrainsMonoNL-Italic.ttf"
  "fonts/JetBrainsMonoNL-Bold.ttf"
  "fonts/JetBrainsMonoNL-BoldItalic.ttf"
  WHENCE "fonts")

FetchContent_Declare(implot URL "${implot_url}")
FetchContent_MakeAvailable(implot)
FetchContent_GetProperties(implot)

add_library(implot STATIC
  "${implot_SOURCE_DIR}/implot.cpp"
  "${implot_SOURCE_DIR}/implot_items.cpp")
target_include_directories(implot PUBLIC "${implot_SOURCE_DIR}")
target_link_libraries(implot PUBLIC imgui::imgui)
add_library(implot::implot ALIAS implot)

add_library(uikit STATIC
  include/uikit/shader_compiler.hpp
  include/uikit/fonts.hpp
  include/uikit/viewport.hpp
  src/fonts.cpp
  src/shader_compiler.cpp
  src/viewport.cpp)
target_include_directories(uikit PUBLIC include)
target_link_libraries(uikit PUBLIC uikit_font_data imgui::imgui imgui::opengl3 implot::implot)

if(NOT EMSCRIPTEN)
  target_link_libraries(uikit PUBLIC imgui::glfw glfw glad::glad)
endif()

add_library(uikit::uikit ALIAS uikit)

add_library(uikit_main STATIC
  include/uikit/main.hpp
  src/main.cpp
  sago/platform_folders.h
  sago/platform_folders.cpp)
target_include_directories(uikit_main PUBLIC include)
target_link_libraries(uikit_main PUBLIC uikit::uikit)
add_library(uikit::main ALIAS uikit_main)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  add_executable(demo WIN32
    demo/main.cpp)
  target_link_libraries(demo PUBLIC uikit::main)
endif()
