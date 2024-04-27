cmake_minimum_required(VERSION 3.14.7)

find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)

pybind11_add_module(uikitpy
  python.cpp
  gl.hpp
  gl.cpp
  glfw.hpp
  glfw.cpp
  imgui.hpp
  imgui.cpp
  implot.hpp
  implot.cpp
  pfd.hpp
  pfd.cpp)

target_link_libraries(uikitpy PUBLIC uikit::uikit)

target_compile_features(uikitpy PRIVATE cxx_std_17)

target_compile_definitions(uikitpy PRIVATE PYBIND11_DETAILED_ERROR_MESSAGES=1)
