cmake_minimum_required(VERSION 3.14.7)

project(uikit)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

if(NOT EMSCRIPTEN)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw URL "https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip")
  FetchContent_MakeAvailable(glfw)
endif()

if(NOT NO_GLAD)
  if(GLAD_GLES3)
    add_subdirectory(gles3)
  else()
    add_subdirectory(gles2)
  endif()
endif()

if(NOT NO_SHADER_COMPILER)
  add_library(shader_compiler
    shader_compiler.hpp
    shader_compiler.cpp)
  target_include_directories(shader_compiler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
  target_link_libraries(shader_compiler PUBLIC glad::glad)
endif()

if(NOT NO_IMGUI)
  FetchContent_Declare(imgui URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9-docking.zip")
  FetchContent_MakeAvailable(imgui)
  FetchContent_GetProperties(imgui)

  add_library(imgui
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp")

  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")

  if(NOT NO_IMGUI_GLFW)
    add_library(imgui_glfw
      "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
      "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
    target_include_directories(imgui_glfw PUBLIC "${imgui_SOURCE_DIR}/backends")
    target_link_libraries(imgui_glfw PUBLIC imgui glfw)
    target_compile_definitions(imgui_glfw PUBLIC GLFW_INCLUDE_NONE=1)
  endif()

  if(NOT NO_IMGUI_OPENGL3)
    add_library(imgui_opengl3
      "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
      "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")
    target_include_directories(imgui_opengl3 PUBLIC "${imgui_SOURCE_DIR}/backends")
    target_link_libraries(imgui_opengl3 PUBLIC imgui glad::glad)
    if(TARGET glad::gles2)
      target_compile_definitions(imgui_opengl3 PUBLIC IMGUI_IMPL_OPENGL_ES2=1)
    elseif(TARGET glad::gles3)
      target_compile_definitions(imgui_opengl3 PUBLIC IMGUI_IMPL_OPENGL_ES3=1)
    endif()
  endif()

endif()

if(NOT NO_IMPLOT)
  FetchContent_Declare(implot URL "https://github.com/epezent/implot/archive/refs/tags/v0.16.zip")
  FetchContent_MakeAvailable(implot)
  FetchContent_GetProperties(implot)

  add_library(implot
    "${implot_SOURCE_DIR}/implot.cpp"
    "${implot_SOURCE_DIR}/implot_items.cpp")
  target_include_directories(implot PUBLIC "${implot_SOURCE_DIR}")
  target_link_libraries(implot PUBLIC imgui)
endif()
