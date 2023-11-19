cmake_minimum_required(VERSION 3.14.7)

project(uikit)

include(cmake/CMakeRC.cmake)

set(glfw_url "https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip"                CACHE STRING "The release URL of GLFW.")
set(imgui_url "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9-docking.zip" CACHE STRING "The release URL of ImGui.")
set(implot_url "https://github.com/epezent/implot/archive/refs/tags/v0.16.zip"         CACHE STRING "The release URL of ImPlot.")

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

if(NOT EMSCRIPTEN)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw URL "${glfw_url}")
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
  add_library(uikit_shader_compiler STATIC
    include/uikit/shader_compiler.hpp
    src/shader_compiler.cpp)
  target_include_directories(uikit_shader_compiler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
  target_link_libraries(uikit_shader_compiler PUBLIC glad::glad)
  add_library(uikit::shader_compiler ALIAS uikit_shader_compiler)
endif()

if(NOT NO_IMGUI)

  FetchContent_Declare(imgui URL "${imgui_URL}")
  FetchContent_MakeAvailable(imgui)
  FetchContent_GetProperties(imgui)

  set(extra_sources)
  set(extra_include_dirs)

  if(NOT NO_IMGUI_STDLIB)
    list(APPEND extra_sources
      "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
      "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")
    list(APPEND extra_include_dirs
      "${imgui_SOURCE_DIR}/misc/cpp")
  endif()

  add_library(imgui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    ${extra_sources})

  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}" ${extra_include_dirs})

  if(NOT NO_IMGUI_GLFW)
    add_library(imgui_glfw STATIC
      "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
      "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
    target_include_directories(imgui_glfw PUBLIC "${imgui_SOURCE_DIR}/backends")
    target_link_libraries(imgui_glfw PUBLIC imgui glfw)
    target_compile_definitions(imgui_glfw PUBLIC GLFW_INCLUDE_NONE=1)
  endif()

  if(NOT NO_IMGUI_OPENGL3)
    add_library(imgui_opengl3 STATIC
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

  if(NOT NO_FONTS)
    cmrc_add_resource_library(imgui_fonts_data
      "${imgui_SOURCE_DIR}/misc/fonts/Cousine-Regular.ttf"
      "${imgui_SOURCE_DIR}/misc/fonts/DroidSans.ttf"
      "${imgui_SOURCE_DIR}/misc/fonts/Karla-Regular.ttf"
      "${imgui_SOURCE_DIR}/misc/fonts/ProggyClean.ttf"
      "${imgui_SOURCE_DIR}/misc/fonts/ProggyTiny.ttf"
      "${imgui_SOURCE_DIR}/misc/fonts/Roboto-Medium.ttf"
      WHENCE "${imgui_SOURCE_DIR}/misc/fonts"
      PREFIX "fonts")
    add_library(imgui_fonts
      include/uikit/imgui_fonts.hpp
      src/imgui_fonts.cpp)
    target_include_directories(imgui_fonts PUBLIC include)
    target_link_libraries(imgui_fonts PUBLIC imgui imgui_fonts_data)
    add_library(uikit::imgui_fonts ALIAS imgui_fonts)
  endif()

endif()

if(NOT NO_IMPLOT)
  FetchContent_Declare(implot URL "${implot_url}")
  FetchContent_MakeAvailable(implot)
  FetchContent_GetProperties(implot)

  add_library(implot STATIC
    "${implot_SOURCE_DIR}/implot.cpp"
    "${implot_SOURCE_DIR}/implot_items.cpp")
  target_include_directories(implot PUBLIC "${implot_SOURCE_DIR}")
  target_link_libraries(implot PUBLIC imgui)
endif()

if(NOT IMGUI_MAIN)

  add_library(uikit_main STATIC
    include/uikit/main.hpp
    src/main.cpp
    sago/platform_folders.h
    sago/platform_folders.cpp)

  target_include_directories(uikit_main PUBLIC include)

  target_link_libraries(uikit_main PUBLIC imgui imgui_glfw imgui_opengl3 imgui_fonts glad::gles2 implot)

  add_library(uikit::main ALIAS uikit_main)

endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  add_executable(demo WIN32
    demo/main.cpp)
  target_link_libraries(demo PUBLIC uikit::main)
endif()
